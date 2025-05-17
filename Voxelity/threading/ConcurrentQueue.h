//
// Created by adrian on 18/05/25.
//

#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H
#include <condition_variable>
#include <mutex>
#include <queue>

template<typename T>
class ConcurrentQueue {
private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void push(const T& item) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(item);
        }
        cv.notify_one();
    }

    bool try_pop(T& result) {
        std::lock_guard<std::mutex> lock(mtx);
        if (queue.empty()) return false;
        result = queue.front();
        queue.pop();
        return true;
    }

    void wait_and_pop(T& result) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return !queue.empty(); });
        result = queue.front();
        queue.pop();
    }
};

#endif //CONCURRENTQUEUE_H
