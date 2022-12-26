#include <iostream>
#include <stdlib.h>
#include "src/tangocount.hpp"
using namespace std;

int main(int argc, char **argv) {
    for (int N = 5000; N <= 1000000; N = min(int(N*1.25), N+50000)) {
        auto tango = TangoTree<int, int>();
        for (int i = 0; i < N; i++) {
            tango.insert(i, i);
        }
        tango.lock();
        for (int i = 0; i < N; i++) {
            tango.find(rand()%(N))->val;
        }
        cout << N << "," << tango.operations << endl;
    }
}