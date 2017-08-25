#include <iostream>
#include <boost/thread.hpp>
#include "../GoStyleChannel.h"

using namespace std;

int main(int argc, char **argv) {
    chan<int> c(5);

    boost::thread send([&]{
        for (int i = 0; i<100; ++i) {
            c << i; // send to channel
            cout << "send: " << i << endl;
        }
        c.close();
    });

    boost::thread receive([&]{
        int j = 0;
        while(j << c) { //receive from channel
            cout << "recv: " << j << endl;
        }
    });

    receive.join();
    return 0;
}
