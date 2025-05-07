#pragma once
#include <cstddef>
#include <vector>
#include <thread>
#include <condition_variable>
#include <queue>
#include <future>
#include <functional>
#include <stdexcept>
#include <mutex>
#include <type_traits>
#include <utility>

class ThreadPool
{

public:
    explicit ThreadPool(std::size_t numThreads = std::thread::hardware_concurrency());
    ~ThreadPool();

    template <class F, class... Args>
    auto enqueue(F &&f, Args &&...args) -> std::future<std::invoke_result_t<F, Args...>>
    {

        using R = std::invoke_result_t<F, Args...>;

        auto taskPtr = std::make_shared<std::packaged_task<R()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<R> fut = taskPtr->get_future();

        {
            std::lock_guard<std::mutex> lk(mutex);
            if (stop) // אם הבריכה נסגרה
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([taskPtr]
                          { (*taskPtr)(); });
        }

        cv.notify_one();

        return fut;
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable cv;
    bool stop = false;
};