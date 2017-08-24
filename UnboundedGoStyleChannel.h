//
// Created by sach_ar on 8/4/17.
//

#ifndef UNBOUNDEDGOSTYLECHANNEL_H
#define UNBOUNDEDGOSTYLECHANNEL_H

#include <list>
#include <mutex>
#include <condition_variable>

template<typename T>
class unbounded_chan {
public:
    unbounded_chan() : closed(false) {}

    void send(const T &t) {
        std::unique_lock<std::mutex> lock(mtx);
        q.push_back(t);
        cond.notify_one();
    }

    bool recv(T &t) {
        std::unique_lock<std::mutex> lock(mtx);
        cond.wait(lock, [&]() { return closed || !q.empty(); });
        if (closed && q.empty()) {
            return false;
        }
        t = q.front();
        q.pop_front();
        return true;
    }

    void close() {
        closed = true;
    }

    bool isClosed() const {
        return closed;
    }

    friend bool operator<<(T &t, unbounded_chan<T> &c) {
        return c.recv(t);
    }

    friend void operator<<(unbounded_chan<T> &c, const T &t) {
        c.send(t);
    }

private:
    std::list<T> q;
    std::mutex mtx;
    std::condition_variable cond;
    bool closed;
};

#endif //UNBOUNDEDGOSTYLECHANNEL_H
