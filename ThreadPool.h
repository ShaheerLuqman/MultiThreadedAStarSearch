#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

using namespace std;

class ThreadPool
{
public:
    // Constructor
    ThreadPool(size_t threads) : stop(false)
    {
        // Create specified number of threads and add them to the worker pool
        for (size_t i = 0; i < threads; ++i)
        {
            workers.emplace_back([this]
                                 {
                // Thread will continue to run until stopped
                for (;;)
                {
                    function<void()> task;

                    {
                        // Lock the task queue
                        unique_lock<mutex> lock(queue_mutex);
                        // Wait for a task to be available or the pool to be stopped
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty())
                            // If pool is stopped and no tasks remaining, exit the thread
                            return;
                        // Get the next task in the queue
                        task = move(tasks.front());
                        tasks.pop();
                    }

                    // Execute the task
                    task();
                } });
        }
    }

    // Enqueue a task to be executed by a worker thread
    template <class F, class... Args>
    auto enqueue(F &&f, Args &&...args) -> future<typename result_of<F(Args...)>::type>
    {
        using return_type = typename result_of<F(Args...)>::type;
        auto task = make_shared<packaged_task<return_type()>>(bind(forward<F>(f), forward<Args>(args)...));
        future<return_type> res = task->get_future();
        {
            // Lock the task queue
            unique_lock<mutex> lock(queue_mutex);
            if (stop)
            {
                // If pool is stopped, don't allow any more tasks to be enqueued
                throw runtime_error("enqueue on stopped ThreadPool");
            }
            // Add the task to the queue
            tasks.emplace([task]
                          { (*task)(); });
        }
        // Notify a worker thread that a task is available
        condition.notify_one();
        return res;
    }

    // Destructor
    ~ThreadPool()
    {
        {
            // Lock the task queue
            unique_lock<mutex> lock(queue_mutex);
            // Set the stop flag to signal worker threads to exit
            stop = true;
        }
        // Notify all worker threads that the pool is stopping
        condition.notify_all();
        // Join all worker threads to wait for them to exit
        for (thread &worker : workers)
        {
            worker.join();
        }
    }

private:
    vector<thread> workers;        // Pool of worker threads
    queue<function<void()>> tasks; // Task queue
    mutex queue_mutex;             // Mutex to lock the task queue
    condition_variable condition;  // Condition variable to signal worker threads
    bool stop;                     // Flag to signal worker threads to exit
};

#endif // THREAD_POOL_H
