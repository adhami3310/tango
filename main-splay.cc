#include <iostream>
#include <stdlib.h>

#include "src/splay.hpp"
using namespace std;

int main(int argc, char **argv) {
    for (int N = 32493; N <= 1000000; N*=2) {
        cout << endl << "Starting " << N << endl; 
        auto splay = SplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            splay.insert(i, i);
        }
        splay.checkIntegrity();
        cout << "finished insert" << endl;
        // splay.pretty_print("  ");
        for (int i = 0; i < N; i++) {
            int x = (rand())%N;
            auto ret = splay.find(x)->val;
            // cout << "finding " << x << endl;
            // splay.pretty_print("  ");
        }
        // splay.checkIntegrity();
        cout << "Finished " << N << endl;
    }
}