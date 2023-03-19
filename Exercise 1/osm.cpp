#include "sys/time.h"
#include "osm.h"

double osm_operation_time(unsigned int iterations) {
    if (iterations <= 0) {
        return -1;
    }

    struct timeval start{}, end{};

    if (gettimeofday(&start, nullptr) == -1) {
        return -1;
    }

    for (unsigned int i = 0; i < iterations; i++) {
        1 + 1;
        1 + 1;
        1 + 1;
        1 + 1;
        1 + 1;
        1 + 1;
        1 + 1;
    }

    if (gettimeofday(&end, nullptr) == -1) {
        return -1;
    }

    unsigned long operations = iterations * 7;

    unsigned long seconds_diff = end.tv_sec - start.tv_sec;
    unsigned long micro_diff = end.tv_usec - start.tv_usec;

    double nano_diff = (seconds_diff * 1000000 + micro_diff) * 1000;

    return nano_diff / operations;
}

void empty_func() {}

double osm_function_time(unsigned int iterations) {
    if (iterations <= 0) {
        return -1;
    }

    struct timeval start{}, end{};

    if (gettimeofday(&start, nullptr) == -1) {
        return -1;
    }

    for (unsigned int i = 0; i < iterations; i++) {
        empty_func();
        empty_func();
        empty_func();
        empty_func();
        empty_func();
        empty_func();
        empty_func();
    }

    if (gettimeofday(&end, nullptr) == -1) {
        return -1;
    }

    unsigned long operations = iterations * 7;

    unsigned long seconds_diff = end.tv_sec - start.tv_sec;
    unsigned long micro_diff = end.tv_usec - start.tv_usec;

    double nano_diff = (seconds_diff * 1000000 + micro_diff) * 1000;

    return nano_diff / operations;
}

double osm_syscall_time(unsigned int iterations) {
    if (iterations <= 0) {
        return -1;
    }

    struct timeval start{}, end{};

    if (gettimeofday(&start, nullptr) == -1) {
        return -1;
    }

    for (unsigned int i = 0; i < iterations; i++) {
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
        OSM_NULLSYSCALL;
    }

    if (gettimeofday(&end, nullptr) == -1) {
        return -1;
    }

    unsigned long operations = iterations * 7;

    unsigned long seconds_diff = end.tv_sec - start.tv_sec;
    unsigned long micro_diff = end.tv_usec - start.tv_usec;

    double nano_diff = (seconds_diff * 1000000 + micro_diff) * 1000;

    return nano_diff / operations;
}
