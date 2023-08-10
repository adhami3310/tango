#include "../src/multisplaycount.hpp"
#include "../src/splaycount.hpp"
#include "../src/tangocount.hpp"

#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>

void testWorking(float startingLogLogN, float maxLogLogN, int K, float additiveStep, int passes, int secondPasses) {
    cout << "# Tango Working Set (" << K << ")" << endl;
    for (float loglogN = startingLogLogN; loglogN <= maxLogLogN; loglogN += additiveStep) {
        long long all_operations = 0;
        int N = pow(2, pow(2, loglogN));
        for (int p = 0; p < secondPasses; p++) {
            std::vector<int> v(N);
            std::iota (std::begin(v), std::end(v), 0);
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(v.begin(), v.end(), g);
            auto tree = TangoTree<int, int>();
            for (int i = 0; i < N; i++) {
                tree.insert(i, i);
            }
            tree.lock();

            for (int startingI = 0; startingI < N; startingI += K) {
                long long operations_bef = tree.operations;
                for (int pass = 0; pass < 10; pass++) {
                    for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                        tree.find(v[startingI + shiftI]);
                    }
                }
                all_operations -= tree.operations-operations_bef;
                for (int pass = 10; pass < passes; pass++) {
                    for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                        tree.find(v[startingI + shiftI]);
                    }
                }
            }

            all_operations += tree.operations;
        }


        cout << N << "\t" << all_operations << endl;
    }

    cout << endl;

    cout << "# MultiSplay Working Set (" << K << ")" << endl;
    for (float loglogN = startingLogLogN; loglogN <= maxLogLogN; loglogN += additiveStep) {
        long long all_operations = 0;
        int N = pow(2, pow(2, loglogN));
        for (int p = 0; p < secondPasses; p++) {

            std::vector<int> v(N);
            std::iota (std::begin(v), std::end(v), 0);
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(v.begin(), v.end(), g);
            auto tree = MultiSplayTree<int, int>();
            for (int i = 0; i < N; i++) {
                tree.insert(i, i);
            }
            tree.lock();

            for (int startingI = 0; startingI < N; startingI += K) {
                long long operations_bef = tree.operations;
                for (int pass = 0; pass < 10; pass++) {
                    for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                        tree.find(v[startingI + shiftI]);
                    }
                }
                all_operations -= tree.operations-operations_bef;
                for (int pass = 10; pass < passes; pass++) {
                    for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                        tree.find(v[startingI + shiftI]);
                    }
                }
            }
            all_operations += tree.operations;
        }
        cout << N << "\t" << all_operations << endl;
    }

    cout << endl;

    cout << "# Splay Working Set (" << K << ")" << endl;
    for (float loglogN = startingLogLogN; loglogN <= maxLogLogN; loglogN += additiveStep) {
        long long all_operations = 0;
        int N = pow(2, pow(2, loglogN));
        for (int p = 0; p < secondPasses; p++) {
            std::vector<int> v(N);
            std::iota (std::begin(v), std::end(v), 0);
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(v.begin(), v.end(), g);
            auto tree = SplayTree<int, int>();
            for (int i = 0; i < N; i++) {
                tree.insert(i, i);
            }
            tree.lock();

            for (int startingI = 0; startingI < N; startingI += K) {
                long long operations_bef = tree.operations;
                for (int pass = 0; pass < 10; pass++) {
                    for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                        tree.find(v[startingI + shiftI]);
                    }
                }
                all_operations -= tree.operations-operations_bef;
                for (int pass = 10; pass < passes; pass++) {
                    for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                        tree.find(v[startingI + shiftI]);
                    }
                }
            }

            all_operations += tree.operations;
        }


        cout << N << "\t" << all_operations << endl;
    }
}
