//
// Created by may on 2019-03-18.
//

#ifndef STLAZYQUEUE_H
#define STLAZYQUEUE_H

#define QUEUE_DEBUG 0


#include <mutex>
#include <queue>
#include <thread>

#include <iostream>
#include <functional>
#include <atomic>
#include "../QtUtil/Utils/Log.h"

//#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds
const unsigned long INTERVAL_MS = 10 ;

template<typename T>
class STLazyQueue {
public:
    inline void init(unsigned long delayMs, std::function<int(STLazyQueue<T> *)> &func) {

        localFunc = [this, func, delayMs]() {

            while (!empty()) {

                std::lock_guard<std::mutex> lock(runner_mutex);
                if(!_run)
                    break;
                unsigned long size = this->size();

                const std::chrono::milliseconds ms(delayMs);
                std::this_thread::sleep_for(ms);

                if(!_run)
                    break;
                unsigned long newSize = this->size();

                if (size != newSize) {

                    continue;
                } else {
                    if(!_run)
                        break;
                    if (!this->empty()) {

//                        int count = this->size();

                        try {
                            if(!_run)
                                break;
                            //
                            func(this);
                        } catch (std::exception &exception) {
                            error_log("error occoured in LazyQueue, {}", exception.what());
                        }

                    }
                }
            }
        };
    }

    explicit STLazyQueue(std::function<int(STLazyQueue<T> *)> &func) {
        init(INTERVAL_MS, func);
    }

    STLazyQueue(unsigned long delayMs, std::function<int(STLazyQueue<T> *)> &func)  {
        init(delayMs, func);
    }

    ~STLazyQueue()
    {
        std::lock_guard<std::mutex> r_lock(runner_mutex);
        _run = false;
    }

    void checkRunner() {
        if(_run && m_queque.size() == 1) {
            std::thread runner(localFunc);
            runner.detach();
        }
    }

    void push(const T &value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queque.push(value);
        checkRunner();
    }

    unsigned long size() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queque.size();
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queque.empty();
    }

    T tail() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queque.back();
    }

    T front() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queque.front();
    }

    void pop() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queque.pop();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        while(!m_queque.empty()) m_queque.pop();;
    }

private:

	unsigned long _delayMs;

    std::atomic<int> _checkCounter;
    std::function<void()> localFunc;
    std::queue<T> m_queque;
    mutable std::mutex m_mutex;
    mutable std::mutex runner_mutex;

    bool _run = true;
};

#endif //STLAZYQUEUE_H
