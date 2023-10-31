#include "Barrier.h"

#include <iostream>
#include <cstdlib>

Barrier::Barrier(int numThreads)
		: mutex(PTHREAD_MUTEX_INITIALIZER)
		, cv(PTHREAD_COND_INITIALIZER)
		, count(0)
		, numThreads(numThreads)
{ }


Barrier::~Barrier()
{
	if (pthread_mutex_destroy(&mutex) != 0) {
        std::cout << "system error: failed to destroy a mutex" << std::endl;
        exit(1);
	}
	if (pthread_cond_destroy(&cv) != 0){
        std::cout << "system error: failed to destroy a condition" << std::endl;
        exit(1);
	}
}


void Barrier::barrier()
{
	if (pthread_mutex_lock(&mutex) != 0){
        std::cout << "system error: failed to lock a mutex" << std::endl;
        exit(1);
	}
	if (++count < numThreads) {
		if (pthread_cond_wait(&cv, &mutex) != 0){
            std::cout << "system error: failed to wait for a condition" << std::endl;
            exit(1);
		}
	} else {
		count = 0;
		if (pthread_cond_broadcast(&cv) != 0) {
            std::cout << "system error: failed to broadcast a condition" << std::endl;
            exit(1);
		}
	}
	if (pthread_mutex_unlock(&mutex) != 0) {
        std::cout << "system error: failed to unlock a mutex" << std::endl;
        exit(1);
	}
}
