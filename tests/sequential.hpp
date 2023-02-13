#include "../src/multisplaycount.hpp"
#include "../src/splaycount.hpp"
#include "../src/tangocount.hpp"

void testSequential(int startingN, int maxN, float geometricFactor, int passes) {
    cout << "#Tango Sequential (" << passes << ")" << endl;
    for (int N = startingN; N <= maxN; N *= geometricFactor) {
        auto tree = TangoTree<int, int>();
        for (int i = 0; i < N; i++) {
            tree.insert(i, i);
        }
        tree.lock();
        for (int i = 0; i < passes * N; i++) {
            tree.find(i%N);
        }
        cout << N << "\t" << tree.operations << endl;
    }

    cout << endl;

    cout << "#MultiSplay Sequential (" << passes << ")" << endl;
    for (int N = startingN; N <= maxN; N *= geometricFactor) {
        auto tree = MultiSplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            tree.insert(i, i);
        }
        tree.lock();
        for (int i = 0; i < passes * N; i++) {
            tree.find(i%N)->val;
        }
        cout << N << "\t" << tree.operations << endl;
    }

    cout << endl;

    cout << "#Splay Sequential (" << passes << ")" << endl;
    for (int N = startingN; N <= maxN; N *= geometricFactor) {
        auto tree = SplayTree<int, int>();
        for (int i = 0; i < N; i++) {
            tree.insert(i, i);
        }
        tree.lock();
        for (int i = 0; i < passes * N; i++) {
            tree.find(i%N)->val;
        }
        cout << N << "\t" << tree.operations << endl;
    }
}
