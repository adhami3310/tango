#include "../src/multisplaycount.hpp"
#include "../src/splaycount.hpp"
#include "../src/tangocount.hpp"

#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>

void testUnified(int startingN, int maxN, int startingK, int maxK, float geometricFactor, int passes) {
    for (int N = startingN; N <= maxN; N *= geometricFactor) {
        for (int K = startingK; K <= min(N, maxK); K *= geometricFactor) {
            {
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

                int finger = rand() % N;
                for (int startingI = 0; startingI < N; startingI += K) {
                    for (int pass = 0; pass < passes; pass++) {
                        for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                            tree.find(v[startingI + shiftI]);
                            finger = (finger + (rand() % K) + 1) % N;
                            tree.find(finger);
                        }
                    }   
                }

                cout << "Tango (" <<  N << ")\t" << tree.operations << endl;
            }
            {
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

                int finger = rand() % N;
                for (int startingI = 0; startingI < N; startingI += K) {
                    for (int pass = 0; pass < passes; pass++) {
                        for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                            tree.find(v[startingI + shiftI]);
                            finger = (finger + (rand() % K) + 1) % N;
                            tree.find(finger);
                        }
                    }   
                }

                cout << "MultiSplay (" <<  N << ")\t" << tree.operations << endl;
            }
            {
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

                int finger = rand() % N;
                for (int startingI = 0; startingI < N; startingI += K) {
                    for (int pass = 0; pass < passes; pass++) {
                        for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                            tree.find(v[startingI + shiftI]);
                            finger = (finger + (rand() % K) + 1) % N;
                            tree.find(finger);
                        }
                    }   
                }

                cout << "Splay (" <<  N << ")\t" << tree.operations << endl;
            }
        }
    }
}
