#include <iostream>
#include <timer.h>
#include <thread>
#include <future>
#include <mutex>
#include <vector>
#include <chrono>

/**
 * The goal of program is to carry out all the tasks. The execution time of a single taskis given in the "time_per_task"
 * list. Write two programs, both single and multi threaded ones, which can complete all the tasks.
 *
 * Hints:
 * How single-threaded program should work:
 * It loops through the list "time_per_task".
 * On each iteration the function "task" is executed.
 *
 * How multi-threaded program should work:
 * Each thread seeks for uncompleted tasks.
 * After finding an uncompleted task the thread blocks the access to that task. (It cannot be done twice by any
 * other thred).
 * After all the tasks have been completed all the threads stop their execution.
 *
 * Write your own functions, mutexes, locks!
 */

std::vector<int> time_per_task{100, 400, 300, 800, 600, 300, 900, 700, 100, 400, 800, 300, 200, 500, 900};
std::vector<bool> completed_tasks(time_per_task.size(), false);
std::vector<bool> pending_tasks(time_per_task.size(), false); //may be useful if a task is started but not completed yet


void task(int time, int task) {
    std::this_thread::sleep_for (std::chrono::milliseconds(time));
    completed_tasks[task] = true;
}

void run_single_threaded() {

}

void run_multi_threaded(int threads){

}

void reset_tasks() {
    for(auto&& task : completed_tasks) {
        task = false;
    }
}

int main() {
    Timer timer;
    int threads = 8;

    timer.start();
    run_single_threaded();
    timer.stop();

    std::cout<<"Task have been completed for one thread! Execution time: " <<timer.elapsed()<< " miliseconds.\n";

    reset_tasks();

    timer.start();
    run_multi_threaded(threads);
    std::cout<<"Task have been completed for " <<threads<<" threads! Execution time: " <<timer.elapsed()<< " miliseconds.\n";
    timer.stop();


    std::cout<<"Complete the task\n";
    return 0;
}