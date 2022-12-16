#include <iostream>

#include "bst.hpp"
using namespace std;

struct TangoNodeInfo {
    // "In addition to storing the key value and depth, each node
    // stores the minimum and maximum depth over the nodes in its subtree."
    int min_depth = -1;
    int max_depth = -1;
    int reference_depth = -1;
    bool marked = false;  // root of an auxiliary tree

    // for maintaining the red-black tree
    bool red = true;
};

template <typename K, typename V>
using TangoNode = BSTNode<K, V, TangoNodeInfo>;

template <typename K, typename V>
class TangoTree : public BST<K, V, TangoNodeInfo> {
   private:
    bool locked = false;

    void left_rotate(TangoNode<K, V> *node) {
        auto child = node->right;
        node->right = child->left;
        if (node->right != nullptr) {
            node->right->parent = node;
        }
        child->parent = node->parent;
        node->parent = child;
        child->left = node;
        if (child->parent == nullptr) {
            this->root = child;
        } else if (child->parent->left == node) {
            child->parent->left = child;
        } else {
            child->parent->right = child;
        }
        if (child->info.marked) throw;
        //swap(child->info.marked, node->info.marked);
    }

    void right_rotate(TangoNode<K, V> *node) {
        auto child = node->left;
        node->left = child->right;
        if (node->left != nullptr) {
            node->left->parent = node;
        }
        child->parent = node->parent;
        node->parent = child;
        child->right = node;
        if (child->parent == nullptr) {
            this->root = child;
        } else if (child->parent->left == node) {
            child->parent->left = child;
        } else {
            child->parent->right = child;
        }
        if (child->info.marked) throw;
        //swap(child->info.marked, node->info.marked);
    }

    void fixupInsert(TangoNode<K, V> *node) {
        this->root->info.red = false;
        // small trees are fixed by default of design
        if (node == nullptr || node->parent == nullptr || node->parent->parent == nullptr)
            return;
        auto parent = node->parent;
        while (parent != nullptr && parent->info.red) {
            auto grandparent = parent->parent;
            if (parent == grandparent->left) {
                auto uncle = grandparent->right;
                if (uncle != nullptr && uncle->info.red) {  // case 1: UNCLE IS RED
                    parent->info.red = false;
                    uncle->info.red = false;
                    grandparent->info.red = true;
                    node = grandparent;
                    parent = grandparent->parent;
                } else {
                    if (node == parent->right) {  // case 2: UNCLE IS BLACK, TRIANGLE
                        left_rotate(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    right_rotate(grandparent);  // case 3: UNCLE IS BLACK, LINE
                    parent->info.red = false;
                    grandparent->info.red = true;
                }
            } else {
                auto uncle = grandparent->left;
                if (uncle != nullptr && uncle->info.red) {  // case 1: UNCLE IS RED
                    parent->info.red = false;
                    uncle->info.red = false;
                    grandparent->info.red = true;
                    node = grandparent;
                    parent = grandparent->parent;
                } else {
                    if (node == parent->left) {  // case 2: UNCLE IS BLACK, TRIANGLE
                        right_rotate(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    left_rotate(grandparent);  // case 3: UNCLE IS BLACK, LINE
                    parent->info.red = false;
                    grandparent->info.red = true;
                }
            }
        }
        this->root->info.red = false;
    }

    void resolveDoubleBlack(TangoNode<K, V> *node) {
        auto parent = node->parent;
        TangoNode<K, V> *sibling = nullptr;
        bool rightChild = false;
        if (parent->right == node) {
            sibling = parent->left;
            rightChild = true;
        } else {
            sibling = parent->right;
        }
        if (sibling && sibling->info.red) {
            if (rightChild) {
                right_rotate(parent);
            } else {
                left_rotate(parent);
            }
            sibling->info.red = false;
            parent->info.red = true;
            resolveDoubleBlack(node);
        } else {
            auto nephewLeft = sibling->left;
            auto nephewRight = sibling->right;
            if ((nephewLeft && nephewLeft->info.red) || (nephewRight && nephewRight->info.red)) {
                bool isRedNephewLeft = (nephewLeft && nephewLeft->info.red);
                auto nephew = isRedNephewLeft ? nephewLeft : nephewRight;
                if (rightChild) {
                    if (!isRedNephewLeft) {
                        nephew->rotate();
                        swap(nephew, sibling);
                    }
                    right_rotate(parent);
                    sibling->info.red = parent->info.red;
                    nephew->info.red = false;
                    parent->info.red = false;
                } else {
                    if (isRedNephewLeft) {
                        nephew->rotate();
                        swap(nephew, sibling);
                    }
                    left_rotate(parent);
                    sibling->info.red = parent->info.red;
                    nephew->info.red = false;
                    parent->info.red = false;
                }
            } else {
                sibling->info.red = true;
                if (parent->info.red) {
                    parent->info.red = false;
                } else if (this->root != parent) {
                    resolveDoubleBlack(parent);
                }
            }
        }
    }

    void removeNode(TangoNode<K, V> *node) {
        if (node == nullptr) return;  // throw?

        if (node->info.red || node->left || node->right) {
            auto child = node->left != nullptr ? node->left : node->right;
            if (child != nullptr) {
                child->parent = node->parent;
                child->info.red = false;
            }
            if (node->parent == nullptr) {
                this->root = child;
            } else if (node->parent->left == node) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
            delete (node);
            return;
        }
        if (node == this->root) {
            this->root = nullptr;
            delete(node);
            return;
        }
        resolveDoubleBlack(node);
        if (node->parent->left == node) {
            node->parent->left = nullptr;
        } else {
            node->parent->right = nullptr;
        }
        delete (node);
    }

    TangoNode<K, V> *getRemoveNode(TangoNode<K, V> *node) {
        if (node == nullptr) throw;
        if (node->left == nullptr || node->right == nullptr) {
            return node;
        }
        auto prev = node->pred();
        this->swapValues(prev, node);
        return getRemoveNode(prev);
    }

    // TangoNode<K, V> *concatenate_left(TangoNode<K, V> *left, TangoNode<K, V> *pivot,
    //                                   TangoNode<K, V> *right);
    // TangoNode<K, V> *concatenate_right(TangoNode<K, V> *left, TangoNode<K, V> *pivot,
    //                                    TangoNode<K, V> *right);
    // TangoNode<K, V> *concatenate(TangoNode<K, V> *root);
    // TangoNode<K, V> *join_helper(TangoNode<K, V> *left, TangoNode<K, V> *root,
    //                              TangoNode<K, V> *right);
    // pair<TangoNode<K, V> *, TangoNode<K, V> *> split_helper(TangoNode<K, V> *root, K
    // key);

    // void split(TangoNode<K, V> *root, TangoNode<K, V> *pivot);
    // pair<TangoNode<K, V> *, TangoNode<K, V> *> split_at(K key);
    // void cut(TangoNode<K, V> *root, int depth);
    // void join(TangoNode<K, V> *root, TangoNode<K, V> *other);

   public:
    TangoTree() : BST<K, V, TangoNodeInfo>() {}

    void print(TangoNode<K, V> *node) {
        if (node == nullptr) {
            cout << "<<Empty>>";
            return;
        }
        auto root = node->info.marked ? "root " : "";
        auto red = node->info.red ? "red " : "black ";
        cout << "<< " << root << red << node->key << ":" << node->val
             << " d:" << node->info.reference_depth << " [" << node->info.min_depth
             << ", " << node->info.max_depth << "] c:";
        print(node->left);
        print(node->right);
        cout << ">>";
    }

    void print() {
        auto locked_string = locked ? "locked " : "unlocked ";
        cout << "{{" << locked_string;
        print(this->root);
        cout << "}}" << endl;
    }

    void printValues(TangoNode<K, V> *node) {
        if (node == nullptr) return;
        printValues(node->left);
        cout << node->val << " ";
        printValues(node->right);
    }

    void printValues() {
        printValues(this->root);
        cout << endl;
    }

    void printGraph(TangoNode<K, V> *node) {
        if (node == nullptr) return;
        if (node->right) {
            cout << node->key << (node->info.red?"r":"") << " " << node->right->key << (node->right->info.red?"r":"")<< endl;
        }
        if (node->left) {
            cout << node->key << (node->info.red?"r":"") << " " << node->left->key << (node->left->info.red?"r":"")<< endl;
        }
        printGraph(node->left);
        printGraph(node->right);
    }

    void printGraph() {
        printGraph(this->root);
        cout << endl;
    }

    void insert(K key, V val) {
        if (locked) throw;
        auto node = new TangoNode(key, val, TangoNodeInfo());
        if (this->root == nullptr) {
            this->root = node;
        } else {
            auto parent = this->root->search(key);
            if (parent->key == key) throw;
            node->parent = parent;
            if (parent->key < key) {
                parent->right = node;
            } else {
                parent->left = node;
            }
        }
        fixupInsert(node);
    }

    void remove(K key) {
        if (locked) throw;
        if (this->root == nullptr) throw;
        auto node = this->root->search(key);
        if (node->key != key) throw;
        node = getRemoveNode(node);
        removeNode(node);
    }

    void lock() { locked = true; }

    void unlock() { locked = false; }

    TangoNode<K, V> *find(K key) {
        if (this->root == nullptr) return nullptr;
        return this->root->search(key);
    }
};