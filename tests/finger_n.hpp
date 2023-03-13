#include "../src/multisplaycount.hpp"
#include "../src/splaycount.hpp"
#include "../src/tangocount.hpp"

void testFinger(int startingN, int maxN, int startingK, int maxK, float geometricFactor, int passes) {
    for (int N = startingN; N <= maxN; N *= geometricFactor) {
        cout << "# Tango Figner (" << N << ")" << endl;
        for (int K = startingK; K <= min(maxK, N); K *= geometricFactor) {
            auto tree = TangoTree<int, int>();
            for (int i = 0; i < N; i++) {
                tree.insert(i, i);
            }
            tree.lock();
            for (int i = rand()%N, x = 0; x < passes * N; i = (i + (rand() % K) + 1) % N, x++) {
                tree.find(i%N);
            }
            cout << K << "\t" << tree.operations << endl;
        }

        cout << endl;
    }

    cout << endl;

    for (int N = startingN; N <= maxN; N *= geometricFactor) {
        cout << "# MultiSplay Finger (" << N << ")" << endl;
        for (int K = startingK; K <= min(maxK, N); K *= geometricFactor) {
            auto tree = MultiSplayTree<int, int>();
            for (int i = 0; i < N; i++) {
                tree.insert(i, i);
            }
            tree.lock();
            for (int i = rand()%N, x = 0; x < passes * N; i = (i + (rand() % K) + 1) % N, x++) {
                tree.find(i%N);
            }
            cout << K << "\t" << tree.operations << endl;
        }

        cout << endl;
    }
    cout << endl;

    for (int N = startingN; N <= maxN; N *= geometricFactor) {
        cout << "# Splay Finger (" << N << ")" << endl;
        for (int K = startingK; K <= min(maxK, N); K *= geometricFactor) {
            auto tree = SplayTree<int, int>();
            for (int i = 0; i < N; i++) {
                tree.insert(i, i);
            }
            tree.lock();
            for (int i = rand()%N, x = 0; x < passes * N; i = (i + (rand() % K) + 1) % N, x++) {
                tree.find(i%N);
            }
            cout << K << "," << tree.operations << endl;
        }
        cout << endl;
    }
}
