#include "../src/multisplaycount.hpp"
#include "../src/splaycount.hpp"
#include "../src/tangocount.hpp"

#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>

void testUnified(float startingLogLogN, float maxLogLogN, int K, float additiveStep) {
    // cout << "# Tango Unified (" << K << ")" << endl;
    // for (float loglogN = startingLogLogN; loglogN <= maxLogLogN; loglogN += additiveStep) {
    //     long long all_operations = 0;
    //     long long accesses = 0;
    //     int N = pow(2, pow(2, loglogN));
    //     auto tree = TangoTree<int, int>();
    //     for (int i = 0; i < N; i++) {
    //         tree.insert(i, i);
    //     }
    //     tree.lock();

    //     for (int iter = 0; iter < N/K; iter += 2*K) {
    //         for (int shiftI = 0; shiftI < K; shiftI++) {
    //             for (int phase = 0; phase < 2*K; phase += K) {
    //                 for (int group = iter; group < N; group += N/K) {
    //                     if (group + shiftI + phase >= N) break;
    //                     tree.find(group + shiftI + phase); 
    //                     accesses++;
    //                 }
    //             }

    //         }
    //     }
    //     cout << N << "\t" << tree.operations/N << endl;
    // }

    // cout << endl;

    // cout << "# MultiSplay Unified (" << K << ")" << endl;
    // for (float loglogN = startingLogLogN; loglogN <= maxLogLogN; loglogN += additiveStep) {
    //     long long all_operations = 0;
    //     long long accesses = 0;
    //     int N = pow(2, pow(2, loglogN));
    //     auto tree = MultiSplayTree<int, int>();
    //     for (int i = 0; i < N; i++) {
    //         tree.insert(i, i);
    //     }
    //     tree.lock();

    //     for (int iter = 0; iter < N/K; iter += 2*K) {
    //         for (int shiftI = 0; shiftI < K; shiftI++) {
    //             for (int phase = 0; phase < 2*K; phase += K) {
    //                 for (int group = iter; group < N; group += N/K) {
    //                     if (group + shiftI + phase >= N) break;
    //                     tree.find(group + shiftI + phase); 
    //                     accesses++;
    //                 }
    //             }

    //         }
    //     }

    //     cout << N << "\t" << tree.operations/N << endl;
    // }

    cout << "# Splay Unified (" << K << ")" << endl;
    for (float loglogN = startingLogLogN; loglogN <= maxLogLogN; loglogN += additiveStep) {
        long long all_operations = 0;
        long long accesses = 0;
        int N = pow(2, pow(2, loglogN));
        auto tree = SplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            tree.insert(i, i);
        }
        tree.lock();

        for (int iter = 0; iter < N/K; iter += 2*K) {
            for (int shiftI = 0; shiftI < K; shiftI++) {
                for (int phase = 0; phase < 2*K; phase += K) {
                    for (int group = iter; group < N; group += N/K) {
                        if (group + shiftI + phase >= N) break;
                        tree.find(group + shiftI + phase); 
                        accesses++;
                    }
                }

            }
        }

        cout << N << "\t" << tree.operations/N << endl;
    }
}

