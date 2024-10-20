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

    // ���캯���������̳߳�
    explicit ThreadPool(size_t num_threads);

    // ��ֹ�����͸�ֵ
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // �����̳߳�
    ~ThreadPool();

    // �ύ����
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
    // �����̺߳���
    void worker_thread();

private:
    std::vector<std::thread> workers; // �����߳�
    std::queue<Task> tasks; // �������
    std::mutex task_mutex; // ����������еĻ�����
    std::condition_variable cv; // ��������
    bool done; // �̳߳��Ƿ����
};

#endif // THREAD_POOL_H