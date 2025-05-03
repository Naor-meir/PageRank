#include "ThreadPool.hpp"
#include <functional>


ThreadPool::ThreadPool(std::size_t numThreads)
{
    if (numThreads == 0)                      
        numThreads = std::thread::hardware_concurrency();  
    if (numThreads == 0)
        numThreads = 1;

    workers.reserve(numThreads);               

    for (std::size_t i = 0; i < numThreads; ++i)
    {
        workers.emplace_back([this]            
        {
            for (;;)
            {
                std::function<void()> task;    

               
                {
                    std::unique_lock<std::mutex> lk(mutex);
                    cv.wait(lk, [this]{ return stop || !tasks.empty(); });

                    if (stop && tasks.empty())  // תנאי יציאה נקי
                        return;

                    task = std::move(tasks.front());
                    tasks.pop();
                }

                
                task();
            }
        });
    }
}
ThreadPool::~ThreadPool()
{
    {   std::lock_guard<std::mutex> lk(mutex);
        stop = true;                
    }
    cv.notify_all();                 

    for (auto& t : workers)          
        if (t.joinable())
            t.join();
}

