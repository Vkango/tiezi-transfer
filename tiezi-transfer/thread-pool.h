#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

class ThreadPool {
public:
    using Task = std::function<void()>;

    // 构造函数：启动线程池
    explicit ThreadPool(size_t num_threads);

    // 禁止拷贝和赋值
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // 销毁线程池
    ~ThreadPool();

    // 提交任务
    template<class FunctionType>
    void submit(FunctionType f) {
        {
            std::unique_lock<std::mutex> lock(task_mutex);
            tasks.emplace(std::move(f));
        }
        cv.notify_one();
    }
    bool has_tasks() const {
        //std::unique_lock<std::mutex> lock(task_mutex);
        return !tasks.empty();
    }
    // 工作线程函数
    void worker_thread();

private:
    std::vector<std::thread> workers; // 工作线程
    std::queue<Task> tasks; // 任务队列
    std::mutex task_mutex; // 保护任务队列的互斥锁
    std::condition_variable cv; // 条件变量
    bool done; // 线程池是否完成
};

#endif // THREAD_POOL_H