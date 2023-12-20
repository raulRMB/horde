#ifndef HORDE_THREADSAFEQUEUE_H
#define HORDE_THREADSAFEQUEUE_H

#include <mutex>
#include <queue>

template<typename T>
class ThreadSafeQueue {
public:
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(value);
    }

    bool pop() {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty()) {
            return false;
        }
        queue.pop();
        return true;
    }

    T front() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.front();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }

private:
    std::queue<T> queue;
    mutable std::mutex mutex;
};

#endif //HORDE_THREADSAFEQUEUE_H
