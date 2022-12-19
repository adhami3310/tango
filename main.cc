#include <iostream>

#include "src/tango.hpp"
using namespace std;

int main(int argc, char **argv) {
    for (int N = 1000000; N <= 1000000; N++) {
        auto tango = TangoTree<int, int>();
        for (int i = 0; i < N; i++) {
            tango.insert(i, i);
        }
        tango.lock();
        for (int i = 0; i < 25*N; i++) {
            tango.find(i%N)->val;
        }
        tango.checkIntegrity();
    }
}