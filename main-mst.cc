#include <iostream>
#include <stdlib.h>
#include "src/multisplaycount.hpp"
using namespace std;

int main(int argc, char **argv) {
    for (int N = 100; N <= 1000000; N*=2) {
        auto multisplay = MultiSplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            multisplay.insert(i, i);
        }
        multisplay.lock();
        // multisplay.checkIntegrity();
        // cout << "Passed insertion" << endl;
        // multisplay.pretty_print("   ", true);

        for (int i = 0; i < N; i++) {
            int x = i % N;
            multisplay.find(x)->val;
            // multisplay.checkIntegrity();
        }
        // multisplay.checkIntegrity();
        cout << N << "," << multisplay.operations << endl;
    }
}