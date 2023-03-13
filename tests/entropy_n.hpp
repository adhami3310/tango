#include "../src/multisplaycount.hpp"
#include "../src/splaycount.hpp"
#include "../src/tangocount.hpp"

#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>
#include "zipfian.hpp"

void testEntropy(int startingN, int maxN, float startingK, float maxK, float geometricFactor, int passes) {

    for (int N = startingN; N <= maxN; N *= 2) {
        cout << "# Tango Entropy (" << N << ")" << endl;
        for (float K = startingK; K <= maxK; K = (1 - (1 - K)/geometricFactor)) {
            std::default_random_engine generator;
            zipfian_int_distribution<int> distribution(1, N, K);
            std::vector<int> v(N);
            std::iota (std::begin(v), std::end(v), 0);
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            shuffle (v.begin(), v.end(), std::default_random_engine(seed));
            std::vector<int> c(N, 0);

            auto tree = TangoTree<int, int>();
            for (int i = 0; i < N; i++) {
                tree.insert(i, i);
            }
            tree.lock();
            for (int x = 0; x < passes * N; x++) {
                int r = v[distribution(generator)];
                tree.find(r % N);
                c[r]++;
            }
            float sum = 0;
            for (int i = 0; i < N; i++) {
                float px = float(c[i]) / (passes * N);
                sum += px * (px > 0? -log(px): 0);
            }
            
            cout << int(sum * (N * passes)) << "\t" << tree.operations << endl;
        }
        cout << endl;
    }

    cout << endl;

    for (int N = startingN; N <= maxN; N *= 2) {
        cout << "# MultiSplay Entropy (" << N << ")" << endl;
        for (float K = startingK; K <= maxK; K = (1 - (1 - K)/geometricFactor)) {
            std::default_random_engine generator;
            zipfian_int_distribution<int> distribution(1, N, K);
            std::vector<int> v(N);
            std::iota (std::begin(v), std::end(v), 0);
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            shuffle (v.begin(), v.end(), std::default_random_engine(seed));
            std::vector<int> c(N, 0);

            auto tree = MultiSplayTree<int, int>();
            for (int i = 0; i < N; i++) {
                tree.insert(i, i);
            }
            tree.lock();
            for (int x = 0; x < passes * N; x++) {
                int r = v[distribution(generator)];
                tree.find(r % N);
                c[r]++;
            }
            float sum = 0;
            for (int i = 0; i < N; i++) {
                float px = float(c[i]) / (passes * N);
                sum += px * (px > 0? -log(px): 0);
            }
            
            cout << int(sum * (N * passes)) << "\t" << tree.operations << endl;
        }
        cout << endl;
    }

    cout << endl;

    for (int N = startingN; N <= maxN; N *= 2) {
        cout << "# Splay Entropy (" << N << ")" << endl;
        for (float K = startingK; K <= maxK; K = (1 - (1 - K)/geometricFactor)) {
            std::default_random_engine generator;
            zipfian_int_distribution<int> distribution(1, N, K);
            std::vector<int> v(N);
            std::iota (std::begin(v), std::end(v), 0);
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            shuffle (v.begin(), v.end(), std::default_random_engine(seed));
            std::vector<int> c(N, 0);

            auto tree = SplayTree<int, int>();
            for (int i = 0; i < N; i++) {
                tree.insert(i, i);
            }
            tree.lock();
            for (int x = 0; x < passes * N; x++) {
                int r = v[distribution(generator)];
                tree.find(r % N);
                c[r]++;
            }
            float sum = 0;
            for (int i = 0; i < N; i++) {
                float px = float(c[i]) / (passes * N);
                sum += px * (px > 0? -log(px): 0);
            }
            
            cout << int(sum * (N * passes)) << "\t" << tree.operations << endl;
        }
        cout << endl;
    }
}
