#include <iostream>
#include <stdlib.h>
#include "tests/sequential.hpp"
// #include "tests/finger.hpp"
// #include "tests/finger_n.hpp"
// #include "tests/random.hpp"
// #include "tests/entropy.hpp"
// #include "tests/entropy_n.hpp"
// #include "tests/working_set.hpp"
// #include "tests/working_set_n.hpp"
// #include "tests/working_set_loglogn.hpp"
// #include "tests/unified.hpp"
// #include "tests/unified_n.hpp"
using namespace std;

int main(int argc, char **argv) {
    testSequential(2, 4096, 2, 25);
    // testFinger(131072, 2, 4096, 2, 25);
    // testFinger(1024, 131072, 1024, 1024, 2, 25);
    // testRandom(10000, 1000000, 1.2, 25);
    // testEntropy(131072/4, 0.1, 0.999999, 1.5, 25);
    // testEntropy(1024, 131072, 0.1, 0.99, 1.5, 25);
    // testWorking(131072, 1, 131072, 2, 100);
    // testWorking(1, 131072, 1, 131072, 2, 100);
    // testWorking(1, 4, 4, 0.05, 100, 20);
    // testUnified(131072, 1, 131072, 2, 25);
    // testUnified(131072, 131072*16, 1024, 1024, 2, 100);

}