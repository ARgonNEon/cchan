#include <iostream>
#include <boost/thread.hpp>
#include "../GoStyleChannel.h"

using namespace std;

void sender(chan<int> &out) {
    for (int i = 0; i<100; ++i) {
        out <<  i; //send to channel
        cout << "send: " << i << endl;
    }
    out.close();
}

void receiver(chan<int> &in) {
    int j = 0;
    while (j << in) { //receive from channel (false if channel closed)
        cout << "recv: " << j << endl;
    }
}

int main(int argc, char **argv) {
    chan<int> c(5);
    boost::thread s(sender, std::ref(c));
    boost::thread r(receiver, std::ref(c));
    r.join();
    return 0;
}
