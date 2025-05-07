#include "ThreadPool.hpp"
#include <functional>

// Constructor: Initializes the thread pool with the specified number of threads
ThreadPool::ThreadPool(std::size_t numThreads)
{
    // If zero is passed, fallback to hardware concurrency
    if (numThreads == 0)
        numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0)
        numThreads = 1; // Fallback again if hardware_concurrency is still zero

    // Reserve space for worker threads
    workers.reserve(numThreads);

    // Launch each worker thread
    for (std::size_t i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this]
                             {
            for (;;)
            {
                std::function<void()> task; // Placeholder for a task to execute

                {
                    // Acquire lock to access the task queue
                    std::unique_lock<std::mutex> lk(mutex);

                    // Wait until a task is available or the pool is stopping
                    cv.wait(lk, [this]{ return stop || !tasks.empty(); });

                    // Exit condition: stop requested and no remaining tasks
                    if (stop && tasks.empty())  
                        return;

                    // Retrieve the next task from the queue
                    task = std::move(tasks.front());
                    tasks.pop();
                }

                // Execute the task outside the locked scope
                task();
            } });
    }
}

// Destructor: Signals threads to stop and joins them
ThreadPool::~ThreadPool()
{
    {
        // Acquire lock and set the stop flag
        std::lock_guard<std::mutex> lk(mutex);
        stop = true;
    }

    // Notify all threads waiting on the condition variable
    cv.notify_all();

    // Join all worker threads
    for (auto &t : workers)
        if (t.joinable())
            t.join();
}
