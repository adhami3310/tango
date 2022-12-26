#include <iostream>
#include <stdlib.h>

#include "src/splay.hpp"
using namespace std;

int main(int argc, char **argv) {
    for (int N = 1000000; N <= 1000000; N++) {
        auto splay = SplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            splay.insert(i, i);
        }
        // cout << "finished insert" << endl;
        // splay.pretty_print("  ");
        for (int i = 0; i < N; i++) {
            int x = (rand())%N;
            auto ret = splay.find(x)->val;
            // cout << "finding " << x << endl;
            // splay.pretty_print("  ");
        }
        splay.checkIntegrity();
    }
}