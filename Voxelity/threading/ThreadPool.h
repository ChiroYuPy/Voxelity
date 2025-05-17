//
// Created by adrian on 18/05/25.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>

#include "ConcurrentQueue.h"
#include "generation/GenerationTask.h"

class ThreadPool {
private:
    std::vector<std::thread> workers;
    ConcurrentQueue<GenerationTask> taskQueue;
    std::atomic<bool> stop;

    void workerLoop() {
        while (!stop) {
            GenerationTask task(nullptr);
            if (taskQueue.try_pop(task)) {
                task.execute();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }
    }

public:
    explicit ThreadPool(const size_t numThreads) : stop(false) {
        for (size_t i=0; i<numThreads; ++i)
            workers.emplace_back([this] { this->workerLoop(); });
    }

    ~ThreadPool() {
        stop = true;
        for (auto& t : workers) {
            if (t.joinable()) t.join();
        }
    }

    void enqueue(const GenerationTask task) {
        taskQueue.push(task);
    }
};

#endif //THREADPOOL_H
