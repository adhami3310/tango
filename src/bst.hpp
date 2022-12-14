// some code copied from
// https://github.com/6851-2021/tango-trees/blob/master/src/bst.hpp

#include <iostream>
using namespace std;

// key, value, info
template <typename K, typename V, typename A>
class BSTNode {
   public:
    K key;
    V val;
    A info;
    BSTNode* left;
    BSTNode* right;
    BSTNode* parent;

    BSTNode(K key, V val)
        : key(key),
          val(val),
          info(A()),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {}

    BSTNode(K key, V val, A info)
        : key(key),
          val(val),
          info(info),
          left(nullptr),
          right(nullptr),
          parent(nullptr) {}

    void print() {
        cout << "(" << key << ", " << val << ", " << info << ")";
        cout << " {";
        if (left != nullptr) {
            left->print();
        }
        cout << ",";
        if (right != nullptr) {
            right->print();
        }
        cout << "}";
    }

    bool is_left_child() { return parent != nullptr && parent->left == this; }

    BSTNode* sibling() {
        if (parent == nullptr) return nullptr;
        if (parent->left == this) return parent->right;
        return parent->left;
    }

    // Replaces this with node by updating only pointers
    // between this and this->parent
    void replace(BSTNode* node) {
        if (parent != nullptr) {
            if (parent->left == this) {
                parent->left = node;
            } else {
                parent->right = node;
            }
        }
        if (node != nullptr) {
            node->parent = parent;
        }
    }

    // Simple tree traversal: returns node with key new_key
    // or leaf node where new_key would be inserted
    BSTNode* search(K new_key) {
        auto current = this;
        while (true) {
            if (current->key == new_key) {
                return current;
            }
            if (current->key > new_key) {
                if (current->left != nullptr) {
                    return current;
                }
                current = current->left;
            } else {
                if (current->right != nullptr) {
                    return current;
                }
                current = current->right;
            }
        }
    }

    // Simple rotation of node with its parent
    void rotate() {
        if (parent == nullptr) return;
        auto p = parent;
        p->replace(this);
        if (parent->left == this) {
            // left rotation
            if (right != nullptr) {
                right->parent = p;
            }
            p->left = right;
            right = p;
        } else {
            // right rotation
            if (left != nullptr) {
                left->parent = p;
            }
            p->right = left;
            left = p;
        }
    }

    // Minimum element in subtree
    BSTNode* min() {
        auto current = this;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Maximum element in subtree
    BSTNode* max() {
        auto current = this;
        while (current->right != nullptr) {
            current = current->right;
        }
        return current;
    }

    // Predecessor
    BSTNode* pred() {
        if (left == nullptr) return nullptr;
        return left->max();
    }

    // Successor
    BSTNode* succ() {
        if (right == nullptr) return nullptr;
        return right->min();
    }

    // Swap places between this and node
    void swap(BSTNode* node) {
        swap(parent, node->parent);
        swap(left, node->left);
        swap(right, node->right);
    }
};

template <typename K, typename V, typename A>
class BST {
   public:
    BST() : root(nullptr) {}

    virtual void rotate(BSTNode<K, V, A>* node) {
        node->rotate();
        if (node->parent == nullptr) {
            root = node;
        }
    }

    // Puts second in place of first and disconnects first
    void replace(BSTNode<K, V, A>* first, BSTNode<K, V, A>* second) {
        if (first->parent != nullptr) {
            if (first->parent->left == this) {
                first->parent->left = second;
            } else {
                first->parent->right = second;
            }
        } else {
            root = second;
        }
        if (second != nullptr) {
            second->parent = first->parent;
        }
    }

    virtual void insert(K key, V val) = 0;

    virtual void remove(K key) = 0;

    virtual BSTNode<K, V, A>* find(K key) = 0;

    // Swaps the places of the two nodes
    void swap(BSTNode<K, V, A>* first, BSTNode<K, V, A>* second) {
        first->swap(second);
        if (root == first) root = second;
        else if(root == second) root = first;
    }

    BSTNode<K, V, A>* root;
};
