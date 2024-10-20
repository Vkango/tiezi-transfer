#include "thread-pool.h"

ThreadPool::ThreadPool(size_t num_threads) : done(false) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back(&ThreadPool::worker_thread, this);
    }
}

ThreadPool::~ThreadPool() {
    done = true;
    cv.notify_all();
    for (auto& worker : workers) {
        worker.join();
    }
}

void ThreadPool::worker_thread() {
    while (!done) {
        Task task;
        {
            std::unique_lock<std::mutex> lock(task_mutex);
            cv.wait(lock, [this] { return !tasks.empty() || done; });
            if (done && tasks.empty()) return;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}