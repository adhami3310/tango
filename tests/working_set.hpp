#include "../src/multisplaycount.hpp"
#include "../src/splaycount.hpp"
#include "../src/tangocount.hpp"

#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>

void testWorking(int N, int startingK, int maxK, float geometricFactor, int passes) {
    cout << "# Tango Working Set (" << passes << ")" << endl;
    for (int K = startingK; K <= maxK; K *= geometricFactor) {
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
            for (int pass = 0; pass < passes; pass++) {
                for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                    tree.find(v[startingI + shiftI]);
                }
            }   
        }

        cout << K << "\t" << tree.operations << endl;
    }

    cout << endl;

    cout << "# MultiSplay Working Set (" << passes << ")" << endl;
    for (int K = startingK; K <= maxK; K *= geometricFactor) {
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
            for (int pass = 0; pass < passes; pass++) {
                for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                    tree.find(v[startingI + shiftI]);
                }
            }   
        }

        cout << K << "\t" << tree.operations << endl;
    }

    cout << endl;

    cout << "# Splay Working Set (" << passes << ")" << endl;
    for (int K = startingK; K <= maxK; K *= geometricFactor) {
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
            for (int pass = 0; pass < passes; pass++) {
                for (int shiftI = 0; shiftI < min(K, N-startingI); shiftI++) {
                    tree.find(v[startingI + shiftI]);
                }
            }   
        }

        cout << K << "\t" << tree.operations << endl;
    }
}
