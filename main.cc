#include "src/bst.hpp"
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
    auto bst_node = BSTNode<int, int, bool>(3, 5);
    bst_node.print();
}