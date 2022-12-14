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
    bool deleted = false;  // for lazy deletions
};

template <typename K, typename V>
using TangoNode = BSTNode<K, V, TangoNodeInfo>;

template <typename K, typename V>
class TangoTree : public BST<K, V, TangoNodeInfo> {
   private:
    bool locked = false;
    int size = 0;
    int deleted = 0;
    void rebuild();
    void left_rotate(TangoNode<K, V> *node);
    void right_rotate(TangoNode<K, V> *node);
    void rotate(TangoNode<K, V> *node);
    void fixup(TangoNode<K, V> *node);
    TangoNode<K, V> *concatenate_left(TangoNode<K, V> *left, TangoNode<K, V> *pivot,
                                      TangoNode<K, V> *right);
    TangoNode<K, V> *concatenate_right(TangoNode<K, V> *left, TangoNode<K, V> *pivot,
                                       TangoNode<K, V> *right);
    TangoNode<K, V> *concatenate(TangoNode<K, V> *root);
    TangoNode<K, V> *join_helper(TangoNode<K, V> *left, TangoNode<K, V> *root,
                                 TangoNode<K, V> *right);
    pair<TangoNode<K, V> *, TangoNode<K, V> *> split_helper(TangoNode<K, V> *root, K key);

    void split(TangoNode<K, V> *root, TangoNode<K, V> *pivot);
    pair<TangoNode<K, V> *, TangoNode<K, V> *> split_at(K key);
    void cut(TangoNode<K, V> *root, int depth);
    void join(TangoNode<K, V> *root, TangoNode<K, V> *other);

   public:
    TangoTree() : BST<K, V, TangoNodeInfo>() {}

    void print(TangoNode<K, V> *node) {
        if (node == nullptr) {
            cout << "<<Empty>>";
            return;
        }
        auto root = node->info->marked ? "root " : "";
        auto red = node->info->red ? "red " : "black ";
        auto deleted = node->info->deleted ? "deleted " : "";
        cout << "<< " << root << red << deleted << node->key << ":" << node->value
             << " d:" << node->info->reference_depth << " [" << node->info->min_depth
             << ", " << node->info->max_depth << "] c:";
        print(node->left);
        print(node->right);
        cout << ">>";
    }

    void print() {
        auto locked_string = locked ? "locked " : "unlocked ";
        print << "{{" << size - deleted << "/" << size << " " << locked_string;
        print(root);
        cout << "}}";
    }

    void insert(K key, V val) {
        if (locked) throw;
        auto node = new TangoNode(key, val, TangoNodeInfo());
        size++;
        if (root == nullptr) {
            root = node;
        } else {
            auto parent = root->search(key);
            if (parent->key == key) throw;
            node->parent = parent;
            if (parent->key < key) {
                parent->right = node;
            } else {
                parent->left = node;
            }
        }
        fixup(node);
    }

    void remove(K key) {
        if (locked) throw;
        if (root == nullptr) throw;
        auto node = root->search(key);
        if (node == nullptr || node->info.deleted) throw;
        deleted++;
        if (deleted > size / 2) {
            rebuild();
        }
    }

    void lock() {
        locked = true;
        rebuild();
    }

    void unlock() {
        rebuild();
        locked = false;
    }

    TangoNode<K, V> *find(K key);
};