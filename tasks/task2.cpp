#include <iostream>
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
    pending_tasks[task] = 1;
    std::this_thread::sleep_for (std::chrono::milliseconds(time));
    completed_tasks[task] = 1;
}

std::mutex task_mtx;

int get_idx_task()
{
    std::lock_guard<std::mutex> lk_guard(task_mtx);
    for (std::vector<int>::size_type i = 0; i != pending_tasks.size(); i++){
        if( pending_tasks[i] != true )
        {
            pending_tasks[i] = true;
            return int(i);
        }
    }
    return -1;
}
void get_task()
{
    while (!std::all_of(completed_tasks.begin(), completed_tasks.end(), [](int i){return i;})){
        int task_idx = get_idx_task();
        if (task_idx != -1){
            std::cout << "Time per task: " << time_per_task[task_idx] << std::endl;
            task(time_per_task[task_idx], task_idx);
            completed_tasks[task_idx] = true;
        }
    }
}


void run_single_threaded() {
    for (std::vector<int>::size_type i = 0; i != time_per_task.size(); i++)
    {
        pending_tasks[i] = true;
        std::cout << "Time per task: " << time_per_task[i] << std::endl;
        task(time_per_task[i], int(i));
        completed_tasks[i] = true;
    }
}

void run_multi_threaded(int threads){
    std::vector<std::thread> thread_pool;
    for(int i = 0; i < threads; i++){
        thread_pool.push_back(std::thread(get_task));
    }
    for ( auto &thread : thread_pool){
        thread.join();
    }
}

void reset_tasks() {
    for (std::vector<int>::size_type i = 0; i != time_per_task.size(); i++)
    {
        completed_tasks[i] = false;
        pending_tasks[i] = false;
    }
}

int main() {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    int threads = 8;
    
    start = std::chrono::system_clock::now();
    run_single_threaded();
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> one_thread = end - start;
    std::cout<<"Task have been completed for one thread! Execution time: " << one_thread.count() << " miliseconds.\n";
    
    reset_tasks();
    
    start = std::chrono::system_clock::now();
    run_multi_threaded(threads);
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> multi_thread = end - start;
    std::cout<<"Task have been completed for " <<threads<<" threads! Execution time: " << multi_thread.count() << " miliseconds.\n";
    
    std::string complete = (multi_thread < one_thread ) ? "Multi thread is faster: " : "Single thread is faster: ";
    double times = (multi_thread < one_thread) ? one_thread / multi_thread : multi_thread / one_thread;
    std::cout << complete << times << " times." << std::endl;
    
    std::cout<<"Complete the task\n";
    return 0;
}
