
#include <iostream>
#include <assert.h>

#include "bst.hpp"
using namespace std;

struct SplayNodeInfo {

};

template <typename K, typename V>
using SplayNode = BSTNode<K, V, SplayNodeInfo>;

template <typename K, typename V>
class SplayTree : public BST<K, V, SplayNodeInfo> {
private:
    bool locked = false;

    void checkIntegrity(SplayNode<K, V> *node) {
        if (node == nullptr) return;
        while (node->left == nullptr || node->right == nullptr) {
            if (node->left) {
                assert(node->key > node->left->key);
                assert(node->key > node->left->max()->key);
                assert(node->left->parent == node);
                node = node->left;
            } else if (node->right) {
                assert(node->key < node->right->key);
                assert(node->key < node->right->min()->key);
                assert(node->right->parent == node);
                node = node->right;
            } else {
                break;
            }
        }

        if (node->left) {
            assert(node->key > node->left->key);
            assert(node->key > node->left->max()->key);
            assert(node->left->parent == node);
        }
        if (node->right) {
            assert(node->key < node->right->key);
            assert(node->key < node->right->min()->key);
            assert(node->right->parent == node);
        }
        checkIntegrity(node->left);
        checkIntegrity(node->right);
    }

    void splayUpto(SplayNode<K, V> *node, SplayNode<K, V> *root) {
        if (node == nullptr || root == nullptr) throw;
        if (node == root) return;
        
        while (true) {
            if (node->parent == nullptr) throw;
            if (node->parent == root) {
                // single rotation
                node->rotate();
                operations++;
                break;
            } else {
                auto par = node->parent;
                if (par->parent == nullptr) throw;

                bool finished = par->parent == root;
                
                if (node->is_left_child() == par->is_left_child()) {
                    operations += 3; // up to grandparent, down to parent
                    // zig-zig and zag-zag
                    par->rotate();
                    node->rotate();
                    operations += 3; // 2 rotate, 1 par->node
                } else {
                    operations += 4; // up to grandparent, down to node
                    // zig-zag and zag-zig
                    node->rotate();
                    node->rotate();
                    operations += 2; // 2 rotate
                }

                if (finished) break;
            }
        }
    }

    void splay(SplayNode<K, V> *node) {
        if (node == nullptr || this->root == nullptr) throw;
        splayUpto(node, this->root);
        this->root = node;
    }

    SplayNode<K, V> *search(SplayNode<K, V> *root, K key) {
        if (root == nullptr) throw;

        auto current = root;
        while (current != nullptr) {
            if (current->key == key) return current;
            if (current->key > key) {
                if (current->left == nullptr) {
                    return current;
                }
                current = current->left;
                operations++;
            } else {
                if (current->right == nullptr) {
                    return current;
                }
                current = current->right;
                operations++;
            }
        }
        return current;
    }

public:
    long long operations = 0;

    SplayTree() : BST<K, V, SplayNodeInfo>() {}

    void lock() {
        locked = true;
        operations = 0;
    }

    void checkIntegrity() {
        if (this->root) {
            assert(this->root->parent == nullptr);
            checkIntegrity(this->root);
        }
    }

    void print(SplayNode<K, V> *node) {
        if (node == nullptr) {
            cout << "<<Empty>>";
            return;
        }
        cout << "<< " << node->key << ":" << node->val << ", c:";
        print(node->left);
        print(node->right);
        cout << ">>";
    }

    void print() {
        cout << "{{";
        print(this->root);
        cout << "}}" << endl;
    }

    void pretty_print(SplayNode<K, V> *node, string prefix, string sep) {
        if (node == nullptr) return;

        string n_prefix = prefix + sep;

        if (node->right) {
            string r_prefix = n_prefix;
            if (node->is_left_child()) {
                r_prefix = prefix + "|" + sep.substr(0, sep.length()-1);
            }
            pretty_print(node->right, r_prefix, sep);
            cout << r_prefix << "/" << endl;
        }
        cout << prefix << node->key << endl;
        if (node->left) {
            string l_prefix = n_prefix;
            if (!node->is_left_child() && node->parent != nullptr) {
                l_prefix = prefix + "|" + sep.substr(0, sep.length()-1);
            }
            cout << l_prefix << "\\" << endl;
            pretty_print(node->left, l_prefix, sep);
        }
    }

    void pretty_print(string sep) {
        pretty_print(this->root, "", sep);
    }

    void insert(K key, V val) {
        if (locked) throw;

        auto node = new SplayNode(key, val, SplayNodeInfo());
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
        splay(node);
    }

    void remove(K key) {
        if (locked) throw;

        if (this->root == nullptr) throw;
        auto node = this->root->search(key);
        if (node->key != key) throw;

        auto parent = node->parent;

        if (node->left == nullptr && node->right == nullptr) {
            if (parent == nullptr) {
                this->root = nullptr;
            } else if (node->is_left_child()) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            delete (node);
            return;
        } else if (node->left == nullptr) {
            node->right->parent = parent;
            if (parent == nullptr) {
                this->root = node->right;
            } else if (node->is_left_child()) {
                parent->left = node->right;
            } else {
                parent->right = node->right;
            }
            delete (node);
        } else if (node->right == nullptr) {
            node->left->parent = parent;
            if (parent == nullptr) {
                this->root = node->left;
            } else if (node->is_left_child()) {
                parent->left = node->left;
            } else {
                parent->right = node->left;
            }
            delete (node);
        } else {
            auto pre = node->pred();
            splayUpto(pre, node);

            pre->right = node->right;
            node->right->parent = pre;

            delete (node);
        }
    }

    SplayNode<K, V>* find(K key) {
        if (!locked) throw;
        if (this->root == nullptr) throw;
        auto found = search(this->root, key);

        if (found->key != key) throw;

        splay(found);
        return found;
    }
};