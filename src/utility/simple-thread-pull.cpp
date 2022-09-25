#include "simple-thread-pull.hpp"

ThreadPool::ThreadPool() {
    if (COUNT_OF_THREADS > std::thread::hardware_concurrency() || COUNT_OF_THREADS <= 0)
        throw std::invalid_argument("Count of threads is invalid");
        
    threads.reserve(COUNT_OF_THREADS);
    for (uint32_t i = 0; i < COUNT_OF_THREADS; i++) {
        threads.emplace_back(&ThreadPool::ThreadLoop, this);
    }
}

ThreadPool::~ThreadPool() {
    quite = true;
    q_cv.notify_all();
    for (auto& t: threads) {
        t.join();
    }
}

void ThreadPool::ThreadLoop() {
    while (!quite) {
        std::unique_lock<std::mutex> lock(qMutex);
        q_cv.wait(lock, [this]()->bool {return !tasks.empty() || quite;});
        if (!tasks.empty() && !quite) {
            std::function<void ()> task = std::move(tasks.front());
            tasks.pop();
            lock.unlock();
            task();
        }
    }
}

void ThreadPool::waitAll() {
    quite = true;
    q_cv.notify_all();
    for (auto& t: threads) {
        t.join();
    }
    quite = false;
    threads.clear();

    for (uint32_t i = 0; i < COUNT_OF_THREADS; i++) {
        threads.emplace_back(&ThreadPool::ThreadLoop, this);
    }
}