#include <iostream>
#include <stdlib.h>

#include "src/multisplay.hpp"
using namespace std;

int main(int argc, char **argv) {
    for (int N = 3; N <= 3; N*=2) {
        auto splay = MultiSplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            splay.insert(i, i);
        }
        splay.lock();
        // splay.checkIntegrity();
        cout << "finished insert" << endl;
        splay.print();
        splay.pretty_print("  ", false);
        for (int i = 0; i < N; i++) {
            int x = (i)%N;
            auto ret = splay.find(x)->val;
            cout << "finding " << x << endl;
            splay.print();
            splay.pretty_print("  ", false);
        }
        // splay.checkIntegrity();
    }
}