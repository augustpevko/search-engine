#include "simple-thread-pull.hpp"

ThreadPool::ThreadPool(): maxThreads(std::thread::hardware_concurrency()),  
                          quite(false) {       
    threads.reserve(maxThreads);
    for (uint32_t i = 0; i < maxThreads; i++)
        threads.emplace_back(&ThreadPool::ThreadLoop, this);
}

ThreadPool::~ThreadPool() {
    quite = true;
    qCv.notify_all();
    for (auto& t: threads)
        t.join();
}

void ThreadPool::ThreadLoop() {
    while (!quite) {
        std::unique_lock<std::mutex> lock(qMutex);
        qCv.wait(lock, [this]()->bool {return !tasks.empty() || quite;});
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
    qCv.notify_all();
    for (auto& t: threads)
        t.join();
    quite = false;
    threads.clear();

    for (uint32_t i = 0; i < maxThreads; i++)
        threads.emplace_back(&ThreadPool::ThreadLoop, this);
}