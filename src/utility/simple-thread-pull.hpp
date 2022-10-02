#pragma once

#include <thread>
#include <functional>
#include <condition_variable>
#include <atomic>
#include <mutex>
#include <queue>

#include "config.hpp"

using Func = std::function<void ()>;

/**
 * @brief The ThreadPool class implements thread pool pattern.
 * It was written for concurrency of inverted index class.
 * It doesn't pretend to be fast.
 * 
 */
class ThreadPool {
public:
    /**
     * @brief Construct a new Thread Pool object.
     * Count of active threads is defined by call std::thread::hardware_concurrency().
     * 
     */
    ThreadPool();
    /**
     * @brief Destroy the Thread Pool object.
     * It joins all threads before being destroyed.
     * 
     */
    ~ThreadPool();

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;

    ThreadPool& operator=(const ThreadPool &) = delete;
    ThreadPool& operator=(ThreadPool &&) = delete;

    /**
     * @brief add task to thread pool.
     * It doesn't return any value.
     *  
     * @param inFunc pointer of function.
     * @param Args parameters of callable function.
     */
    template <class ...T, typename ...FuncTypes>
    void addTask(void (*inFunc)(FuncTypes...), T&&... Args) {
        auto task = std::bind(inFunc, Args...);
        std::unique_lock<std::mutex> lock(qMutex);
        tasks.push(task);
        lock.unlock();
        qCv.notify_one();
    }
    /**
     * @brief wait for all functions to finish.
     * 
     */
    void waitAll();

private:
    /**
     * @brief loop for thread vector.
     * 
     */
    void ThreadLoop();

    size_t maxThreads{1};
    std::atomic<bool> quite{false};
    std::mutex qMutex;                 
    std::condition_variable qCv; 
    std::vector<std::thread> threads;
    std::queue<Func> tasks;
};