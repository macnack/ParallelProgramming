#include <iostream>
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
int zeros_in_number(int number);

int run_single_threaded() {
    int sum = 0;
    for( int i = 1; i <= n; i++){
        sum += zeros_in_number(i);
    }
    return sum;
}
int sum_zeros_in_number(int start, int stop){
    int sum = 0;
    for( int i = start; i < stop; i++){
        sum += zeros_in_number(i);
    }
    return sum;
}

int run_multi_threaded(int threads){
    int chunks = n / threads;
    std::vector<std::future<int>> futures;
    for (int i = 0; i < threads; i++) {
        std::future<int> temp = std::async(sum_zeros_in_number, (i*chunks)+1, ((i+1)*chunks)+1);
        futures.emplace_back(std::move(temp));
    }
    int sum = 0;
    for ( auto &thread : futures){
        sum += thread.get();
    }

    return sum;
}

int zeros_in_number(int number) {
    std::string number_str = std::to_string(number);
    int zeros = 0;
    for( std::string::iterator it = number_str.begin(); it != number_str.end(); it++){
        if ( *it == '0' ){
            zeros += 1;
        }
    }
    return zeros;
}

int main() {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::cout << std::thread::hardware_concurrency() << std::endl;
    int threads = 8;
    int zeros;
    start = std::chrono::system_clock::now();
    zeros = run_single_threaded();
    end = std::chrono::system_clock::now();
    std::cout<<"Number of zeros in the range from 1 to : " <<n<< " equals "<<zeros<<".\n";
    std::chrono::duration<double> elapsed_time_single_thread = end - start;
    std::cout<<"Execution time: " << elapsed_time_single_thread.count()<< " miliseconds.\n";

    start = std::chrono::system_clock::now();
    zeros = run_multi_threaded(threads);
    end = std::chrono::system_clock::now();
    std::cout<<"Number of zeros in the range from 1 to : " <<n<< " equals "<<zeros<<".\n";
    std::chrono::duration<double> elapsed_time_multi_thread = end - start;
    std::cout<<"Execution time: " << elapsed_time_multi_thread.count() << " miliseconds. \n";
    std::string complete = (elapsed_time_multi_thread < elapsed_time_single_thread ) ? "Multi thread is faster: " : "Single thread is faster: ";
    double times = (elapsed_time_multi_thread < elapsed_time_single_thread) ? elapsed_time_single_thread / elapsed_time_multi_thread : elapsed_time_multi_thread / elapsed_time_single_thread;
    std::cout << complete << times << " times." << std::endl;
    return 0;
}
