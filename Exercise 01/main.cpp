#include <iostream>
#include "osm.h"

int main (int argc, char **argv)
{
    double time = osm_operation_time(10000);
    std::cout << "Time taken: " << time << " nanoseconds" << std::endl;

    time = osm_function_time(10000);
    std::cout << "Time taken: " << time << " nanoseconds" << std::endl;

    time = osm_syscall_time(10000);
    std::cout << "Time taken: " << time << " nanoseconds" << std::endl;

    return 0;
}
