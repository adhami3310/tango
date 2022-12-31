#include <iostream>
#include <stdlib.h>

#include "src/splaycount.hpp"
using namespace std;

int main(int argc, char **argv) {
    for (int N = 100; N <= 1000000; N*=2) {
        auto splay = SplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            splay.insert(i, i);
        }
        splay.lock();
        // splay.checkIntegrity();
        // cout << "finished insert" << endl;
        // splay.pretty_print("  ");
        for (int i = 0; i < N; i++) {
            int x = (i)%N;
            auto ret = splay.find(x)->val;
            // cout << "finding " << x << endl;
            // splay.pretty_print("  ");
        }
        // splay.checkIntegrity();
        cout << N << "," << splay.operations << endl;
    }
}