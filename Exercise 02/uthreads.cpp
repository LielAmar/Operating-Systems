#include "uthreads.h"

#include <iostream>
#include <list>
#include <queue>
#include <csignal>
#include <csetjmp>
#include <memory>
#include <cstring>
#include <sys/time.h>

#define AVAILABLE false
#define UNAVAILABLE true

#define JB_SP 6
#define JB_PC 7


typedef unsigned long address_t;

enum NextThreadCause {
    TERMINATE,
    BLOCK,
    CYCLE
};


void main_entry_point () {}
void timer_handler(int sig);


class Thread {

public:

    unsigned int tid;
    char *stack;

    sigjmp_buf env{};
    thread_entry_point entry_point;

    unsigned int thread_quantum_counter;
    unsigned int remaining_sleep_quantums;
    bool is_blocked;

    Thread (const Thread &other)
    {
        this->tid = 0;
        this->stack = nullptr;

        this->entry_point = nullptr;

        this->thread_quantum_counter = 0;
        this->remaining_sleep_quantums = 0;
        this->is_blocked = false;

        *this = other;
    }

    Thread (unsigned int tid, thread_entry_point entry_point)
    {
        this->tid = tid;
        this->stack = new char[STACK_SIZE];

        this->entry_point = entry_point;

        this->thread_quantum_counter = 0;
        this->remaining_sleep_quantums = 0;
        this->is_blocked = false;


        // Initializing the SP, PC and ENV
        auto sp = (address_t) &stack + STACK_SIZE - sizeof (address_t);
        auto pc = (address_t) entry_point;

        sigsetjmp(this->env, 1);

        this->env->__jmpbuf[JB_SP] = translate_address (sp);
        this->env->__jmpbuf[JB_PC] = translate_address (pc);

        // Resets the env masks
        sigemptyset (&this->env->__saved_mask);
    }

    Thread &operator= (const Thread &other)
    {
        if (&other == this)
        {
            return *this;
        }

        delete[] this->stack;

        this->tid = other.tid;
        this->stack = new char[STACK_SIZE];
        std::memcpy (this->stack, other.stack, STACK_SIZE);

        this->entry_point = other.entry_point;

        this->thread_quantum_counter = other.thread_quantum_counter;
        this->remaining_sleep_quantums = other.remaining_sleep_quantums;
        this->is_blocked = other.is_blocked;

        // Initializing the SP, PC and ENV
        auto sp = (address_t) &this->stack + STACK_SIZE - sizeof (address_t);

        sigsetjmp(this->env, 1);

        this->env->__jmpbuf[JB_SP] = translate_address (sp);
        this->env->__jmpbuf[JB_PC] = other.env->__jmpbuf[JB_PC];

        // Resets the env masks
        this->env->__saved_mask = other.env->__saved_mask;

        return *this;
    }

    ~Thread ()
    {
        delete[] this->stack;
        this->stack = nullptr;
    }

private:

    static address_t translate_address (address_t addr)
    {
        address_t ret;
        asm volatile("xor    %%fs:0x30,%0\n"
                     "rol    $0x11,%0\n"
                : "=g" (ret)
                : "0" (addr));
        return ret;
    }

};


class ThreadsManager {

public:

    unsigned int quantum_usecs;
    unsigned int quantums_counter;

    struct itimerval it_val{};

    Thread * running_thread;
    std::deque<Thread *> ready{};
    std::list<Thread *> blocked{};

    bool available_threads[MAX_THREAD_NUM] = {AVAILABLE};

    ThreadsManager () : ThreadsManager (0)
    {}

    explicit ThreadsManager (unsigned int quantum_usecs)
    {
        this->quantum_usecs = quantum_usecs;
        this->quantums_counter = 1;

        this->running_thread = new Thread(0, main_entry_point);

        // Setting the first quantum of thread 0 to be 1
        this->running_thread->thread_quantum_counter++;

        this->available_threads[0] = UNAVAILABLE;
    }

    /**
     *
     */
    void start_clock() {
        struct sigaction sa = {nullptr};

        // Install timer_handler as the signal handler for SIGVTALRM.
        sa.sa_handler = &timer_handler;
        if (sigaction(SIGVTALRM, &sa, nullptr) < 0)
        {
            std::cerr << "system error: sigaction has failed while subscribing to the SIGTVALRM signal" << std::endl;
            exit (1);
        }

        it_val.it_value.tv_sec = this->quantum_usecs / 1000000;
        it_val.it_value.tv_usec = this->quantum_usecs % 1000000;
        it_val.it_interval = it_val.it_value;

        reset_clock();
    }

    /**
     *
     */
    void reset_clock() const {
        if(setitimer(ITIMER_VIRTUAL, &it_val, nullptr) < 0) {
            std::cerr << "system error: couldn't set the itimer virtual timer" << std::endl;
            exit (1);
        }
    }

    /**
     *
     */
    void free_process ()
    {
        while (!this->ready.empty ())
        {
            this->ready.back()->~Thread();
//            delete this->ready.back();
            this->ready.pop_back();
        }

        while (!this->blocked.empty ())
        {
            this->blocked.back()->~Thread();
//            delete this->blocked.back();
            this->blocked.pop_back();
        }

        this->running_thread->~Thread();
    }


    /**
    * @brief
    * Checks if the given tid is attached to an existing thread
    *
    * @param tid   ID of the thread to check if exists
    * @return      Whether a thread with the given tid exists
    */
    bool thread_exists (int tid)
    {
        if(!legal_tid(tid))
        {
            return false;
        }

        return this->available_threads[tid] == UNAVAILABLE;
    }

    /**
     * @brief
     * Returns the best available thread index, going from 1 all the way to MAX_THREAD_NUM
     *
     * @return ID between [1, MAX_THREAD_NUM) if we can assign a new thread, -1 if failed
     */
    int next_available_index ()
    {
        for (int i = 1; i < MAX_THREAD_NUM; i++)
        {
            if (this->available_threads[i] == AVAILABLE)
            {
                return i;
            }
        }

        return -1;
    }

    /**
     * @brief
     * Creates a new thread and inserts to the "Ready" queue (ready deque)
     *
     * @param entry_point   Thread's entry point
     * @return ID of the newly created thread, -1 if failed
     */
    int create_new_thread (thread_entry_point entry_point)
    {
        if (entry_point == nullptr)
        {
            std::cerr << "thread library error: the given entry point is null" << std::endl;
            return -1;
        }

        int tid = this->next_available_index ();

        if (tid == -1)
        {
            std::cerr << "thread library error: you reached the max number of threads" << std::endl;
            return -1;
        }

        this->available_threads[tid] = UNAVAILABLE;

        try
        {
            auto* thread = new Thread(tid, entry_point);
            this->ready.push_back (thread);
        }
        catch (const std::bad_alloc &exception)
        {
            this->free_process ();

            std::cerr << "system error: memory allocation for a new thread failed" << std::endl;
            exit (1);
        }

        return tid;
    }

    /**
     * Terminates the process with the given tid
     *
     * @param tid   ID of the thread to check if exists
     * @return      0 if termination was successful, -1 otherwise
     */
    int terminate_thread (int tid)
    {
        if(!legal_tid(tid)) {
            std::cerr << "thread library error: the given tid is illegal" << std::endl;
            return -1;
        }

        if (!thread_exists (tid))
        {
            std::cerr << "thread library error: no thread with the given tid exists" << std::endl;
            return -1;
        }

        if (tid == 0)
        {
            free_process ();
            exit (0);
        }

        this->available_threads[tid] = AVAILABLE;

        // Blocks the correct thread from the ready data-structure (if present)
        for (auto iter = this->ready.begin (); iter != this->ready.end ();)
        {
            if ((*iter)->tid == (unsigned int) tid)
            {
                (*iter)->~Thread();
                this->ready.erase (iter);
                return 0;
            }
            else
            {
                ++iter;
            }
        }

        // Blocks the correct thread from the blocked data-structure (if present)
        for (auto iter = this->blocked.begin (); iter != this->blocked.end ();)
        {
            if ((*iter)->tid == (unsigned int) tid)
            {
                (*iter)->~Thread();
                this->blocked.erase (iter);
                return 0;
            }
            else
            {
                ++iter;
            }
        }

        // Otherwise, the terminated thread is the currently running one
        this->next_thread (TERMINATE);
        return 0;
    }

    /**
     *
     * @param tid
     */
    int block_thread (int tid)
    {
        if(!legal_tid(tid)) {
            std::cerr << "thread library error: the given tid is illegal" << std::endl;
            return -1;
        }

        if (!thread_exists (tid))
        {
            std::cerr << "thread library error: no thread with the given tid exists" << std::endl;
            return -1;
        }

        if (tid == 0)
        {
            std::cerr << "thread library error: cannot block the main (0) thread" << std::endl;
            return -1;
        }

        if (is_blocked (tid))
        {
            return 0;
        }

        // Blocks the correct thread from the ready data-structure (if present)
        for (auto iter = this->ready.begin (); iter != this->ready.end ();)
        {
            if ((*iter)->tid == (unsigned int) tid)
            {
                Thread* t = *iter;
                t->is_blocked = true;

                this->blocked.push_back (t);
                this->ready.erase (iter);
                return 0;
            }
            else
            {
                ++iter;
            }
        }

        // Blocks the correct thread from the sleeping (blocked) data-structure (if present)
        for (auto iter = this->blocked.begin (); iter != this->blocked.end ();)
        {
            if ((*iter)->tid == (unsigned int) tid)
            {
                Thread* t = *iter;
                t->is_blocked = true;
                return 0;
            }
            else
            {
                ++iter;
            }
        }

        // Otherwise, the blocked thread is the currently running one
        this->running_thread->is_blocked = true;
        this->next_thread (BLOCK);

        return 0;
    }

    /**
     *
     * @param tid
     * @return
     */
    int resume_thread (int tid)
    {
        if(!legal_tid(tid)) {
            std::cerr << "thread library error: the given tid is illegal" << std::endl;
            return -1;
        }

        if (!thread_exists (tid))
        {
            std::cerr << "thread library error: no thread with the given tid exists" << std::endl;
            return -1;
        }

        for (auto iter = this->blocked.begin (); iter != this->blocked.end ();)
        {
            if ((*iter)->tid == (unsigned int) tid)
            {
                (*iter)->is_blocked = false;

                if((*iter)->remaining_sleep_quantums == 0) {
                    this->ready.push_back (*iter);
                    this->blocked.erase (iter);
                }

                return 0;
            }
            else
            {
                ++iter;
            }
        }

        return 0;
    }

    /**
 *
 * @param sleep_quantums
 */
    int sleep_thread (int sleep_quantums)
    {
        if (sleep_quantums < 0)
        {
            std::cerr << "thread library error: cannot sleep a thread with a negative sleep quantums" << std::endl;
            return -1;
        }

        if(running_thread->tid == 0) {
            std::cerr << "thread library error: cannot sleep the main (0) thread" << std::endl;
            return -1;
        }

        this->running_thread->remaining_sleep_quantums = sleep_quantums;
        this->next_thread (BLOCK);

        return 0;
    }

    /**
     *
     * @param tid
     * @return
     */
    int get_thread_quantums(int tid) {
        if(!legal_tid(tid)) {
            std::cerr << "thread library error: the given tid is illegal" << std::endl;
            return -1;
        }

        if (!thread_exists (tid))
        {
            std::cerr << "thread library error: no thread with the given tid exists" << std::endl;
            return -1;
        }

        // Returns the correct quantums of thread from the ready data-structure (if present)
        for (auto iter = ready.begin (); iter != ready.end ();)
        {
            if ((*iter)->tid == (unsigned int) tid)
            {
                return (int) (*iter)->thread_quantum_counter;
            }
            else
            {
                ++iter;
            }
        }

        // Returns the correct quantums of thread from the blocked data-structure (if present)
        for (auto iter = blocked.begin (); iter != blocked.end ();)
        {
            if ((*iter)->tid == (unsigned int) tid)
            {
                return (int) (*iter)->thread_quantum_counter;
            }
            else
            {
                ++iter;
            }
        }

        // Otherwise, the desired thread is the currently running one
        return (int) running_thread->thread_quantum_counter;
    }

    /**
     *
     * @param cause
     */
    void next_thread (NextThreadCause cause)
    {
        // Looping over every timely-blocked quantum and reducing 1 from it's blocked time.
        for (auto iter = this->blocked.begin (); iter != this->blocked.end ();)
        {
            if ((*iter)->remaining_sleep_quantums > 0)
            {
                (*iter)->remaining_sleep_quantums -= 1;
            }

            if ((*iter)->remaining_sleep_quantums == 0 && !(*iter)->is_blocked)
            {
                this->ready.push_back (*iter);
                iter = this->blocked.erase (iter);
            } else {
                ++iter;
            }
        }

        // If ready is empty:
        // - update blocked, update counters
        if (cause == CYCLE)
        {
            if (this->ready.empty ())
            {
                // Updating quantums counters and trackers
                this->quantums_counter += 1;
                this->running_thread->thread_quantum_counter += 1;

                return;
            }

            Thread* thread = this->running_thread;

            this->ready.push_back (thread);

            // Save old thread's state. If it was called through a setlongjmp, return
            // (because needs to continue it's work).
            int ret_val = sigsetjmp(thread->env, 1);

            if(ret_val != 0) {
                return;
            }
        }
        else if(cause == BLOCK)
        {
            Thread* thread = this->running_thread;

            this->blocked.push_back (thread);

            // Save old thread's state. If it was called through a setlongjmp, return
            // (because needs to continue it's work).
            int ret_val = sigsetjmp(thread->env, 1);

            if(ret_val != 0) {
                return;
            }
        }
        else if(cause == TERMINATE)
        {
            (*this->running_thread).~Thread();
            this->running_thread = nullptr;
        }

        this->running_thread = this->ready.front();
        this->ready.pop_front ();

        // Updating quantums counters and trackers
        this->quantums_counter += 1;
        this->running_thread->thread_quantum_counter += 1;

        // Reset the clock for the next thread
        reset_clock();

        // Jump to the saved environment of the new thread
        siglongjmp (this->running_thread->env, 1);
    }

private:

    static bool legal_tid(int tid) {
        return tid >= 0 && tid < MAX_THREAD_NUM;
    }

    bool is_blocked (int tid)
    {
        for (auto iter = this->blocked.begin (); iter != this->blocked.end ();)
        {
            if ((*iter)->tid == (unsigned int) tid && (*iter)->is_blocked)
            {
                return true;
            }
            else
            {
                ++iter;
            }
        }

        return false;
    }
};


ThreadsManager threads_manager;

void timer_handler(int sig)
{
    threads_manager.next_thread(CYCLE);
}

void block_alarm() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &set, nullptr);
}

void unblock_alarm() {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &set, nullptr);
}


int uthread_init(int quantum_usecs)
{
    if (quantum_usecs < 0)
    {
        std::cerr << "thread library error: the given quantum_usecs argument must be non-negative" << std::endl;
        return -1;
    }

    threads_manager = ThreadsManager (quantum_usecs);
    threads_manager.start_clock();

    return 0;
}

int uthread_spawn (thread_entry_point entry_point)
{
    block_alarm();
    int result = threads_manager.create_new_thread (entry_point);
    unblock_alarm();

    return result;
}

int uthread_terminate (int tid)
{
    block_alarm();
    int result = threads_manager.terminate_thread (tid);
    unblock_alarm();

    return result;
}

int uthread_block (int tid)
{
    block_alarm();
    int result = threads_manager.block_thread (tid);
    unblock_alarm();

    return result;
}

int uthread_resume (int tid)
{
    block_alarm();
    int result = threads_manager.resume_thread (tid);
    unblock_alarm();

    return result;
}

int uthread_sleep (int num_quantums)
{
    block_alarm();
    int result = threads_manager.sleep_thread(num_quantums);
    unblock_alarm();

    return result;
}

int uthread_get_tid ()
{
    return (int) threads_manager.running_thread->tid;
}

int uthread_get_total_quantums ()
{
    return (int) threads_manager.quantums_counter;
}

int uthread_get_quantums (int tid)
{
    block_alarm();
    int result = threads_manager.get_thread_quantums(tid);
    unblock_alarm();

    return result;
}