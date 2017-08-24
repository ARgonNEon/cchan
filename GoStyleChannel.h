//
// Created by sach_ar on 8/2/17.
//

#ifndef VISUAL_TACTILE_OBJECT_REGISTRATION_GOSTYLECHANNEL_H
#define VISUAL_TACTILE_OBJECT_REGISTRATION_GOSTYLECHANNEL_H

#include <list>
#include <mutex>
#include <condition_variable>

template<typename T>
class chan {
public:
    chan(long cap) : closed(false), fill(0, closed), empty(cap, closed) {}

    chan() : chan(1) {}

    void close() {
        closed = true;
        empty.cond.notify_all();
        fill.cond.notify_all();
    }

    bool isClosed() const {
        return closed;
    }

    friend bool operator<<(T &t, chan<T> &c) {
        return c.recv(t);
    }

    friend void operator<<(chan<T> &c, const T &t) {
        c.send(t);
    }

protected:
    class semaphore { // Dijkstra's semaphore
        friend class chan;

    public:
        semaphore(long n, const bool &closed) : n(n), closed(closed) {}

        void v() {
            std::unique_lock<std::mutex> lock(mtx);
            ++n;
            cond.notify_all();
        }

        void p() {
            std::unique_lock<std::mutex> lock(mtx);
            cond.wait(lock, [&]() { return n > 0 || closed; });
            --n;
        }

    protected:
        std::mutex mtx;
        std::condition_variable cond;
        const bool &closed;
        long n;
    };


    void send(const T &t) {
        empty.p();
        { //synchronized by mutex
            std::unique_lock<std::mutex> lock(mutex);
            if (closed) {
                throw std::runtime_error("Send on closed channel");
            }
            q.push_back(t);
        }
        fill.v();
    }

    bool recv(T &t) {
        fill.p();
        { //synchronized by mutex
            std::unique_lock<std::mutex> lock(mutex);
            if (closed && q.empty()) {
                return false;
            }
            t = q.front();
            q.pop_front();
        }
        empty.v();
        return true;
    }

private:
    std::list<T> q;
    std::mutex mutex;
    semaphore fill, empty;
    bool closed;
};

#endif //VISUAL_TACTILE_OBJECT_REGISTRATION_GOSTYLECHANNEL_H
