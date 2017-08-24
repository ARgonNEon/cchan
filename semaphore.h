//
// Created by sach_ar on 8/3/17.
//

#ifndef SEMAPHORE_H_H
#define SEMAPHORE_H_H

/**
 * Dijktra's semaphore
 */
class semaphore {
public:
    semaphore(long n) : n(n) {}

    void v() {
        std::unique_lock <std::mutex> lock(mtx);
        ++n;
        cond.notify_one();
    }

    void p() {
        std::unique_lock <std::mutex> lock(mtx);
        while (n == 0) {
            cond.wait(lck);
        }
        --n;
    }

private:
    std::mutex mtx;
    std::condition_variable cond;
    long n;
};

#endif //SEMAPHORE_H_H
