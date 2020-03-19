#include <iostream>
#include <timer.h>
#include <thread>
#include <future>
#include <mutex>
#include <vector>
#include <chrono>

/**
 * Calculate the sum of of 0's in the range from 1 to n. For example the number 10230 has 2 zeros.
 *
 * Hints:
 * How single-threaded program should work:
 * It loops through each number from 1 to n
 *
 *
 * How multi-threaded program should work:
 * Split the range from 1 to n int equal chunks
 * Each thread calculates the sum of zeros from Begin to End
 *
 * Write your own functions, mutexes, locks!
 */

int n = 1000000;

int run_single_threaded() {

}

int run_multi_threaded(int threads){

}

int zeros_in_number(int number) {
    /*
     * implement that
     */
    return -1;
}

int main() {
    Timer timer;
    int threads = 8;
    int zeros;

    timer.start();
    zeros = run_single_threaded();
    timer.stop();
    std::cout<<"Number of zeros in the range from 1 to : " <<n<< " equals "<<zeros<<".\n";
    std::cout<<"Execution time: " <<timer.elapsed()<< " miliseconds.\n";


    timer.start();
    zeros = run_multi_threaded(threads);

    std::cout<<"Number of zeros in the range from 1 to : " <<n<< " equals "<<zeros<<".\n";
    std::cout<<"Execution time: " <<timer.elapsed()<< " miliseconds.\n";
    timer.stop();


    std::cout<<"Complete the task\n";
    return 0;
}