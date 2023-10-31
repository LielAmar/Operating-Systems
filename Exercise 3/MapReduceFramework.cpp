#include "MapReduceFramework.h"
#include "Barrier.h"

#include <iostream>
#include <algorithm>
#include <pthread.h>
#include <semaphore.h>
#include <atomic>


typedef struct ThreadContext ThreadContext;
typedef struct JobContext JobContext;
typedef struct IntermediateContext IntermediateContext;

void* start_thread_job(void* arg);


// A struct representing the context of a whole job
struct JobContext {
    const MapReduceClient* client;

    // General
    bool waited;

    // Threads
    int number_of_threads;
    pthread_t* threads;
    ThreadContext* threads_context;

    // Input & Output
    const InputVec* input_data;
    std::vector<IntermediateVec*>* shuffled_data;
    OutputVec* output_data;

    // Mutexes, Semaphores, Barriers and Atomic variables
    std::atomic<uint64_t>* state;
    std::atomic<int>* map_stage_atomic_counter;
    std::atomic<int>* number_of_intermediate_elements;
    std::atomic<int>* number_of_shuffled_elements;
    std::atomic<int>* number_of_output_elements;
    sem_t* progress_semaphore;
    sem_t* reduce_semaphore;
    Barrier* barrier;
};

// A struct representing the context of a single thread within a job
struct ThreadContext {
    unsigned int thread_id;

    JobContext* job_context;
    IntermediateVec* intermediate_data;
};

// A struct representing the context of a single emit2 call
struct IntermediateContext {
    JobContext* job_context;
    ThreadContext* thread_context;
};


JobHandle startMapReduceJob(const MapReduceClient& client,
                            const InputVec& inputVec, OutputVec& outputVec,
                            int multiThreadLevel) {
    // Initializing the Job Context
    auto job_context = new JobContext ();
    job_context->client = &client;

    job_context->waited = false;

    job_context->number_of_threads = multiThreadLevel;
    job_context->threads = new pthread_t[multiThreadLevel];
    job_context->threads_context = new ThreadContext[multiThreadLevel];

    job_context->input_data = &inputVec;
    job_context->shuffled_data = nullptr;
    job_context->output_data = &outputVec;

    job_context->state = new std::atomic<uint64_t>((long) inputVec.size() << 31 |
                                                   (long) UNDEFINED_STAGE << 62);
    job_context->map_stage_atomic_counter = new std::atomic<int>(0);
    job_context->number_of_intermediate_elements = new std::atomic<int>(0);
    job_context->number_of_shuffled_elements = new std::atomic<int>(0);
    job_context->number_of_output_elements = new std::atomic<int>(0);
    job_context->progress_semaphore = new sem_t ();
    job_context->reduce_semaphore = new sem_t ();
    job_context->barrier = new Barrier(multiThreadLevel);

    // Initializing semaphores
    if(sem_init(job_context->progress_semaphore, 0, 1) != 0) {
        std::cout << "system error: failed to init a semaphore" << std::endl;
        exit(1);
    }

    if(sem_init(job_context->reduce_semaphore, 0, 1) != 0) {
        std::cout << "system error: failed to init a semaphore" << std::endl;
        exit(1);
    }

    // Creating the threads...
    for (int i = 0; i < multiThreadLevel; ++i) {
        job_context->threads_context[i].thread_id = i;
        job_context->threads_context[i].job_context = job_context;
        job_context->threads_context[i].intermediate_data = new IntermediateVec();
    }

    for (int i = 0; i < multiThreadLevel; ++i) {
        if(pthread_create(job_context->threads + i, nullptr,
                       start_thread_job, &job_context->threads_context[i]) != 0) {
            std::cout << "system error: failed to create a thread" << std::endl;
            exit(1);
        }
    }

    job_context->state->exchange((long) inputVec.size() << 31 |
                                 (long) MAP_STAGE << 62);

    return (JobHandle) job_context;
}

void* start_thread_job(void* arg) {
    auto* thread_context = (ThreadContext*) arg;
    JobContext* job_context = thread_context->job_context;

    // ===== Executing map stage =====
    unsigned long old_value = (*(job_context->map_stage_atomic_counter))++;

    while(old_value < job_context->input_data->size()) {
        // Executes the #map function on the current pair
        InputPair pair = job_context->input_data->at(old_value);

        auto* context = new IntermediateContext();
        context->job_context = job_context;
        context->thread_context = thread_context;

        job_context->client->map(pair.first, pair.second, context);
        delete context;

        // Update state value
        (*(job_context->state))++;

        // Getting the next pair to process
        old_value = (*(job_context->map_stage_atomic_counter))++;
    }

    // Sorting the intermediate data of each thread
    std::sort(thread_context->intermediate_data->begin(), thread_context->intermediate_data->end(),
              [](IntermediatePair& p1, IntermediatePair& p2) {
        return *p1.first < *p2.first;
    });


    // ===== Executing shuffle stage =====

    // Making all threads wait at the same point, then wait for thread 0 to shuffle, and continue afterwards.
    job_context->barrier->barrier();

    if(thread_context->thread_id == 0) {
        // Resets the state
        job_context->state->exchange((long) job_context->number_of_intermediate_elements->load() << 31 |
                                     (long) SHUFFLE_STAGE << 62);

        auto* shuffled_data = new std::vector<IntermediateVec*>();

        // Find max key on every iteration and create a new vector for all elements with the same, max, key.
        // Then push it to shuffled_data, and update the shuffle state.
        while(true) {
            // find max key
            K2* max_key = nullptr;

            for(int i = 0; i < job_context->number_of_threads; i++) {
                if ((*(job_context->threads_context + i)->intermediate_data).empty()) {
                    continue;
                }

                K2* local_key = (*(job_context->threads_context + i)->intermediate_data).back().first;

                if (max_key == nullptr || *max_key < *local_key) {
                    max_key = local_key;
                }
            }

            // If max_key is null, we don't have any key => we have finished looping over all keys
            if(max_key == nullptr) {
                break;
            }

            auto* key_data = new IntermediateVec();

            for(int i = 0; i < job_context->number_of_threads; i++) {
                IntermediateVec* intermediate_data = (job_context->threads_context + i)->intermediate_data;

                while (!intermediate_data->empty() && !(*intermediate_data->back().first < *max_key) &&
                                                     !(*max_key < *intermediate_data->back().first)) {
                    key_data->push_back(intermediate_data->back());
                    intermediate_data->pop_back();

                    // Update state value
                    (*(job_context->state))++;
                }
            }

            // Insert the new vector to shuffled_data, and update the number of shuffled elements
            shuffled_data->push_back(key_data);
            (*(job_context->number_of_shuffled_elements))++;
        }

        job_context->shuffled_data = shuffled_data;

        // Resets the state
        job_context->state->exchange((long) job_context->number_of_shuffled_elements->load() << 31 |
                                     (long) REDUCE_STAGE << 62);
    }

    job_context->barrier->barrier();


    // ===== Executing reduce stage =====

    while (!job_context->shuffled_data->empty()) {
        // Trying to pop an element from the shuffled data (after locking the semaphore)
        if(sem_wait(job_context->reduce_semaphore) != 0) {
            std::cout << "system error: failed to wait a semaphore" << std::endl;
            exit(1);
        }

        if(job_context->shuffled_data->empty()) {
            if(sem_post(job_context->reduce_semaphore) != 0) {
                std::cout << "system error: failed to post a semaphore" << std::endl;
                exit(1);
            }
            break;
        }

        IntermediateVec* vec_pairs = job_context->shuffled_data->back();
        job_context->shuffled_data->pop_back();

        if(sem_post(job_context->reduce_semaphore) != 0) {
            std::cout << "system error: failed to post a semaphore" << std::endl;
            exit(1);
        }

        // Call reduce on the won element from the shuffled data.
        job_context->client->reduce(vec_pairs, job_context);

        delete vec_pairs;
    }

    return nullptr;
}


void emit2 (K2* key, V2* value, void* context) {
    auto* intermediateContext = (IntermediateContext*) context;
    intermediateContext->thread_context->intermediate_data->emplace_back(key, value);
    (*(intermediateContext->job_context->number_of_intermediate_elements))++;
}

void emit3 (K3* key, V3* value, void* context) {
    auto* job_context = (JobContext *) context;

    // Make sure only one thread can write to the output at a time, and update the state
    if(sem_wait(job_context->progress_semaphore) != 0) {
        std::cout << "system error: failed to wait a semaphore" << std::endl;
        exit(1);
    }

    job_context->output_data->emplace_back(key, value);

    if(sem_post(job_context->progress_semaphore) != 0) {
        std::cout << "system error: failed to post a semaphore" << std::endl;
        exit(1);
    }

    // Updating the number of output elements & state value
    (*(job_context->number_of_output_elements))++;
    (*(job_context->state))++;
}

void waitForJob(JobHandle job) {
    JobContext* job_context = ((JobContext *) job);

    if(job_context == nullptr) {
        return;
    }

    // Prevents calling pthread_join twice
    if(job_context->waited) {
        return;
    }

    job_context->waited = true;

    for(int i = 0; i < job_context->number_of_threads; i++) {
        if(pthread_join(job_context->threads[i], nullptr) != 0) {
            std::cout << "system error: failed to join a thread" << std::endl;
            exit(1);
        }
    }
}

void getJobState(JobHandle job, JobState* state) {
    JobContext* job_context = ((JobContext *) job);

    uint64_t value = job_context->state->load();

    state->stage = static_cast<stage_t>(value >> 62);
    unsigned long denominator = value >> 31 & (0x7FFFFFFF);
    unsigned long numerator = value & (0x7FFFFFFF);

    state->percentage = ((float) numerator / (float) denominator) * 100;
}

void closeJobHandle(JobHandle job) {
    JobContext* job_context = ((JobContext *) job);

    if(job_context == nullptr) {
        return;
    }

    // Wait for all threads to finish their job
    waitForJob(job);

    delete job_context->map_stage_atomic_counter;
    delete job_context->number_of_intermediate_elements;
    delete job_context->number_of_shuffled_elements;
    delete job_context->number_of_output_elements;

    if(sem_destroy(job_context->reduce_semaphore) != 0) {
        std::cout << "system error: failed to destroy a semaphore" << std::endl;
        exit(1);
    }

    if(sem_destroy(job_context->progress_semaphore) != 0) {
        std::cout << "system error: failed to destroy a semaphore" << std::endl;
        exit(1);
    }

    delete job_context->reduce_semaphore;
    delete job_context->progress_semaphore;
    delete job_context->barrier;

    for (int i = 0; i < job_context->number_of_threads; ++i) {
        delete job_context->threads_context[i].intermediate_data;
    }

    delete job_context->shuffled_data;
    delete job_context->state;

    delete[] job_context->threads;
    delete[] job_context->threads_context;

    delete job_context;
}