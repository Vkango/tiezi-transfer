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
template<class FunctionType>
void ThreadPool::submit(FunctionType f) {
    {
        std::unique_lock<std::mutex> lock(task_mutex);
        tasks.emplace(std::move(f));
    }
    cv.notify_one();
}

template void ThreadPool::submit<std::function<void()>>(std::function<void()>);