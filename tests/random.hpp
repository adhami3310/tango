#include "../src/multisplaycount.hpp"
#include "../src/splaycount.hpp"
#include "../src/tangocount.hpp"

void testRandom(int startingN, int maxN, float geometricFactor, int passes) {
    cout << "# Tango Random (" << passes << ")" << endl;
    for (int N = startingN; N <= maxN; N = max(int(N*geometricFactor), N+startingN)) {
        auto tree = TangoTree<int, int>();
        for (int i = 0; i < N; i++) {
            tree.insert(i, i);
        }
        tree.lock();
        for (int i = 0; i < passes * N; i++) {
            tree.find(rand()%N);
        }
        cout << N << "\t" << tree.operations << endl;
    }

    cout << endl;

    cout << "# MultiSplay Random (" << passes << ")" << endl;
    for (int N = startingN; N <= maxN; N = max(int(N*geometricFactor), N+startingN)) {
        auto tree = MultiSplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            tree.insert(i, i);
        }
        tree.lock();
        for (int i = 0; i < passes * N; i++) {
            tree.find(rand()%N)->val;
        }
        cout << N << "\t" << tree.operations << endl;
    }

    cout << endl;

    cout << "# Splay Random (" << passes << ")" << endl;
    for (int N = startingN; N <= maxN; N = max(int(N*geometricFactor), N+startingN)) {
        auto tree = SplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            tree.insert(i, i);
        }
        tree.lock();
        for (int i = 0; i < passes * N; i++) {
            tree.find(rand()%N)->val;
        }
        cout << N << "\t" << tree.operations << endl;
    }
}
