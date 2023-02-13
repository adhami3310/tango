#include "../src/multisplaycount.hpp"
#include "../src/splaycount.hpp"
#include "../src/tangocount.hpp"

void testFinger(int N, int startingK, int maxK, float geometricFactor, int passes) {
    cout << "#Tango Figner (" << passes << ")" << endl;
    for (int K = startingK; K <= maxK; K *= geometricFactor) {
        auto tree = TangoTree<int, int>();
        for (int i = 0; i < N; i++) {
            tree.insert(i, i);
        }
        tree.lock();
        for (int i = 0, x = 0; x < passes * N; i = (i + (rand() % K) + 1) % N, x++) {
            tree.find(i%N);
        }
        cout << K << "\t" << tree.operations << endl;
    }

    cout << endl;

    cout << "#MultiSplay Finger (" << passes << ")" << endl;
    for (int K = startingK; K <= maxK; K *= geometricFactor) {
        auto tree = MultiSplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            tree.insert(i, i);
        }
        tree.lock();
        for (int i = 0, x = 0; x < passes * N; i = (i + (rand() % K) + 1) % N, x++) {
            tree.find(i%N);
        }
        cout << K << "\t" << tree.operations << endl;
    }

    cout << endl;

    cout << "#Splay Finger (" << passes << ")" << endl;
    for (int K = startingK; K <= maxK; K *= geometricFactor) {
        auto tree = SplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            tree.insert(i, i);
        }
        tree.lock();
        for (int i = 0, x = 0; x < passes * N; i = (i + (rand() % K) + 1) % N, x++) {
            tree.find(i%N);
        }
        cout << K << "\t" << tree.operations << endl;
    }
}
