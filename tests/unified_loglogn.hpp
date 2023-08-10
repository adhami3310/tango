#include "../src/multisplaycount.hpp"
#include "../src/splaycount.hpp"
#include "../src/tangocount.hpp"

#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>

void testUnified(float startingLogLogN, float maxLogLogN, int maxK, float additiveStep) {
    for (int K = maxK; K <= maxK; K *= 2) {
        // cout << "# Tango Unified (" << K << ")" << endl;
        // for (float loglogN = startingLogLogN; loglogN <= maxLogLogN; loglogN += additiveStep) {
        //     long long all_operations = 0;
        //     long long accesses = 0;
        //     int N = pow(2, pow(2, loglogN));
        //     if (N < 2*2*K*K) continue;
        //     auto tree = TangoTree<int, int>();
        //     for (int i = 0; i < N; i++) {
        //         tree.insert(i, i);
        //     }
        //     tree.lock();

        //     for (int iter = 0; iter < N/K; iter += 2*K) {
        //         long long operations_bef = tree.operations;
        //         for (int phase = 0; phase < 2*K; phase += K) {
        //             for (int group = iter; group < N; group += N/K) {
        //                 if (group + phase >= N) break;
        //                 tree.find(group + phase);
        //             }
        //         }
        //         all_operations -= tree.operations - operations_bef;
        //         for (int shiftI = 1; shiftI < K; shiftI++) {
        //             for (int phase = 0; phase < 2*K; phase += K) {
        //                 for (int group = iter; group < N; group += N/K) {
        //                     if (group + shiftI + phase >= N) break;
        //                     tree.find(group + shiftI + phase); 
        //                     accesses++;
        //                 }
        //             }
        //         }
        //     }

        //     all_operations += tree.operations;

        //     cout << "(" << N << ", " << 1.0f*all_operations/accesses << ")," << endl;
        // }

        // cout << endl;

        // cout << "# MultiSplay Unified (" << K << ")" << endl;
        // for (float loglogN = startingLogLogN; loglogN <= maxLogLogN; loglogN += additiveStep) {
        //     long long all_operations = 0;
        //     long long accesses = 0;
        //     int N = pow(2, pow(2, loglogN));
        //     if (N < 2*2*K*K) continue;
        //     auto tree = MultiSplayTree<int, int>();
        //     for (int i = 0; i < N; i++) {
        //         tree.insert(i, i);
        //     }
        //     tree.lock();

        //     for (int iter = 0; iter < N/K; iter += 2*K) {
        //         long long operations_bef = tree.operations;
        //         for (int phase = 0; phase < 2*K; phase += K) {
        //             for (int group = iter; group < N; group += N/K) {
        //                 if (group + phase >= N) break;
        //                 tree.find(group + phase);
        //             }
        //         }
        //         all_operations -= tree.operations - operations_bef;
        //         for (int shiftI = 1; shiftI < K; shiftI++) {
        //             for (int phase = 0; phase < 2*K; phase += K) {
        //                 for (int group = iter; group < N; group += N/K) {
        //                     if (group + shiftI + phase >= N) break;
        //                     tree.find(group + shiftI + phase); 
        //                     accesses++;
        //                 }
        //             }
        //         }
        //     }

        //     all_operations += tree.operations;

        //     cout << "(" << N << ", " << 1.0f*all_operations/accesses << ")," << endl;
        // }

        // cout << endl;

        cout << "# Splay Unified (" << K << ")" << endl;
        for (float loglogN = startingLogLogN; loglogN <= maxLogLogN; loglogN += additiveStep) {
            long long all_operations = 0;
            long long accesses = 0;
            int N = pow(2, pow(2, loglogN));
            if (N < 2*2*K*K) continue;
            auto tree = SplayTree<int, int>();
            for (int i = 0; i < N; i++) {
                tree.insert(i, i);
            }
            tree.lock();

            vector<long long> values;

            for (int iter = 0; iter < N/K; iter += 2*K) {
                long long operations_bef = tree.operations;
                for (int phase = 0; phase < 2*K; phase += K) {
                    for (int group = iter; group < N; group += N/K) {
                        if (group + phase >= N) break;
                        tree.find(group + phase);
                        values.push_back(group + phase);
                        accesses++;
                    }
                }
                // all_operations -= tree.operations - operations_bef;
                for (int shiftI = 1; shiftI < K; shiftI++) {
                    for (int phase = 0; phase < 2*K; phase += K) {
                        for (int group = iter; group < N; group += N/K) {
                            if (group + shiftI + phase >= N) break;
                            tree.find(group + shiftI + phase); 
                            values.push_back(group + phase + shiftI);
                            accesses++;
                        }
                    }
                }
            }


            long long total = 0;
            for (int i = 0; i < values.size(); i++) {
                long long spec_min = N;
                for (int j = i-1; j >= 0; j--) {
                    spec_min = min(spec_min, abs(values[j]-values[i]) + abs(j-i));
                    if (spec_min < 2*K + 2 && i-j > 4*K) {
                        break;
                    }
                }
                total += spec_min;
            }

            all_operations += tree.operations;

            cout << "(" << N << ", " << 1.0f*all_operations/N << ", " << 1.0f * total / N << ")," << endl;
        }

        cout << endl;
    }
}

