#include <iostream>
#include <stdlib.h>
// #include "tests/sequential.hpp"
// #include "tests/finger.hpp"
// #include "tests/random.hpp"
#include "tests/entropy.hpp"
using namespace std;

int main(int argc, char **argv) {
    // testSequential(4096, 20000000, 2, 25);
    // testFinger(131072, 2, 4096, 2, 25);
    // testRandom(10000, 1000000, 1.2, 25);
    testEntropy(131072/4, 0.1, 0.999999, 1.5, 25);
}