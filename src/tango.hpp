#include <iostream>
#include <vector>
#include <assert.h>
#include <set>

#include "bst.hpp"
using namespace std;

struct TangoNodeInfo {
    // "In addition to storing the key value and depth, each node
    // stores the minimum and maximum depth over the nodes in its subtree."
    int min_depth = -1;
    int max_depth = -1;
    int reference_depth = -1;
    int black_height = 1;
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
        swap(child->info.marked, node->info.marked);
        updateMinMaxDepth(node);
        updateMinMaxDepth(child);
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
        swap(child->info.marked, node->info.marked);
        updateMinMaxDepth(node);
        updateMinMaxDepth(child);
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

    TangoNode<K, V> *fixupMergeTango(TangoNode<K, V> *node) {
        // small trees are fixed by default of design
        if (node == nullptr || node->parent == nullptr || node->parent->parent == nullptr)
            throw;
        auto parent = node->parent;
        while (!node->info.marked && parent->info.red) {
            auto grandparent = parent->parent;
            if (parent == grandparent->left) {
                auto uncle = grandparent->right;
                if (uncle != nullptr && uncle->info.red) {  // case 1: UNCLE IS RED
                    parent->info.red = false;
                    uncle->info.red = false;
                    parent->info.black_height++;
                    uncle->info.black_height++;
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
                    parent->info.black_height++;
                    grandparent->info.black_height--;
                }
            } else {
                auto uncle = grandparent->left;
                if (uncle != nullptr && uncle->info.red) {  // case 1: UNCLE IS RED
                    parent->info.red = false;
                    uncle->info.red = false;
                    parent->info.black_height++;
                    uncle->info.black_height++;
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
                    parent->info.black_height++;
                    grandparent->info.black_height--;
                }
            }
        }
        while (node->parent && !node->info.marked) {
            node = node->parent;
        }
        if (node->info.red) {
            node->info.red = false;
            node->info.black_height++;
        }
        return node;
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
            if ((nephewLeft && nephewLeft->info.red) ||
                (nephewRight && nephewRight->info.red)) {
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
        if (node == nullptr) throw;

        // node has at most one child
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
            delete (node);
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

    TangoNode<K, V> *concatenate_left(TangoNode<K, V> *left, TangoNode<K, V> *pivot,
                                      TangoNode<K, V> *right, TangoNode<K, V> *initParent,
                                      bool leftChild, bool marked) {
        // left > right
        auto current = left;
        while (current->info.black_height > right->info.black_height) {
            current = current->right;
        }
        auto parent = current->parent;
        parent->right = pivot;
        pivot->parent = parent;
        pivot->left = current;
        current->parent = pivot;
        pivot->right = right;
        right->parent = pivot;
        pivot->info.red = true;
        pivot->info.black_height = current->info.black_height;
        left->info.marked = true;
        left->parent = initParent;
        if (initParent == nullptr) {
            this->root = left;
        } else if (leftChild) {
            initParent->left = left;
        } else {
            initParent->right = left;
        }
        left->info.marked = true;
        if (current->info.red) {
            left = fixupMergeTango(current);
        } else if (parent->info.red) {
            left = fixupMergeTango(pivot);
        }
        updateMinMaxDepthUp(pivot);
        left->info.marked = marked;
        return left;
    }
    TangoNode<K, V> *concatenate_right(TangoNode<K, V> *left, TangoNode<K, V> *pivot,
                                       TangoNode<K, V> *right, TangoNode<K, V> *initParent,
                                       bool leftChild, bool marked) {
        // right > left
        auto current = right;
        while (current->info.black_height > left->info.black_height) {
            current = current->left;
        }
        auto parent = current->parent;
        parent->left = pivot;
        pivot->parent = parent;
        pivot->right = current;
        current->parent = pivot;
        pivot->left = left;
        left->parent = pivot;
        pivot->info.red = true;
        pivot->info.black_height = current->info.black_height;
        right->parent = initParent;
        if (initParent == nullptr) {
            this->root = right;
        } else if (leftChild) {
            initParent->left = right;
        } else {
            initParent->right = right;
        }
        right->info.marked = true;
        if (current->info.red) {
            right = fixupMergeTango(current);
        } else if (parent->info.red) {
            right = fixupMergeTango(pivot);
        }
        updateMinMaxDepthUp(pivot);
        right->info.marked = marked;
        return right;
    }

    bool isChildNil(TangoNode<K, V> *node) {
        return node == nullptr || node->info.marked;
    }

    int sizeTango(TangoNode<K, V> *node) {
        int result = 1;
        if (!isChildNil(node->left)) {
            result += sizeTango(node->left);
        }
        if (!isChildNil(node->right)) {
            result += sizeTango(node->right);
        }
        return result;
    }

    set<K> depthsTango(TangoNode<K, V> *node) {
        set<K> result = {node->info.reference_depth};
        if (!isChildNil(node->left)) {
            for (auto a: depthsTango(node->left)) {
                result.insert(a);
            }
        }
        if (!isChildNil(node->right)) {
            for (auto a: depthsTango(node->right)) {
                result.insert(a);
            }
        }
        return result;
    }

    void checkIntegrity(TangoNode<K, V> *node) {
        if (node == nullptr) return;
        assert(node->info.reference_depth >= node->info.min_depth && node->info.reference_depth <= node->info.max_depth);
        auto parent = node->parent;
        if (parent) {
            if (!node->info.marked) {
                assert(!node->info.red || !parent->info.red);
                assert(node->info.min_depth >= parent->info.min_depth && node->info.max_depth <= parent->info.max_depth);
            } else {
                assert(!node->info.red);
            }
        } else {
            assert(this->root == node);
            assert(node->info.marked);
            assert(!node->info.red);
        }
        assert(depthsTango(node).size() == sizeTango(node));
        if (node->left) {
            assert(node->key > node->left->max()->key);
            assert(node->left->parent == node);
        }
        if (node->right) {
            assert(node->key < node->right->min()->key);
            assert(node->right->parent == node);
        }
        if (isChildNil(node->left) && isChildNil(node->right)) {
            assert(node->info.black_height == !node->info.red);
            assert(node->info.reference_depth == node->info.min_depth && node->info.reference_depth == node->info.max_depth);
        } else if (isChildNil(node->right)) {
            assert(node->info.black_height == node->left->info.black_height + !node->info.red);
            assert(min(node->info.reference_depth, node->left->info.min_depth) == node->info.min_depth && max(node->info.reference_depth, node->left->info.max_depth) == node->info.max_depth);
        } else if (isChildNil(node->left)) {
            assert(node->info.black_height == node->right->info.black_height + !node->info.red);
            assert(min(node->info.reference_depth, node->right->info.min_depth) == node->info.min_depth && max(node->info.reference_depth, node->right->info.max_depth) == node->info.max_depth);
        } else {
            assert(node->info.black_height == node->left->info.black_height + !node->info.red);
            assert(node->info.black_height == node->right->info.black_height + !node->info.red);
            assert(min(min(node->info.reference_depth, node->right->info.min_depth), node->left->info.min_depth) == node->info.min_depth && max(node->info.reference_depth, max(node->right->info.max_depth, node->left->info.max_depth)) == node->info.max_depth);
            for (int i = max(node->right->info.min_depth, node->left->info.min_depth); i <= min(node->right->info.max_depth, node->left->info.max_depth); i++) {
                assert(maxWithDepth(node->left, i-1) == maxTango(node->left));
                assert(minWithDepth(node->right, i-1) == minTango(node->right));
            }
        }
        checkIntegrity(node->left);
        checkIntegrity(node->right);
    }

    TangoNode<K, V> *concatenate(TangoNode<K, V> *node) {
        auto initParent = node->parent;
        bool leftChild = initParent && initParent->left == node;
        auto previousNode = node;
        bool marked = node->info.marked;
        node->info.marked = false;

        if (isChildNil(node->left) && isChildNil(node->right)) {
            node->info.red = false;
            node->info.black_height = 1;
            node->info.marked = marked;
            updateMinMaxDepth(node);
            return node;
        } else if (isChildNil(node->left)) {
            auto right = node->right;
            auto parent = minTango(right);
            node->right = parent->left;
            if (parent->left) parent->left->parent = node;
            parent->left = node;
            right->parent = node->parent;
            node->parent = parent;
            node->info.red = true;
            node->info.black_height = 0;
            right->parent = initParent;
            if (initParent == nullptr) {
                this->root = right;
            } else if (leftChild) {
                initParent->left = right;
            } else {
                initParent->right = right;
            }
            right->info.marked = true;
            updateMinMaxDepthUp(node);
            if (node->parent->info.red) right = fixupMergeTango(node);
            right->info.marked = marked;
            return right;
        } else if (isChildNil(node->right)) {
            auto left = node->left;
            auto parent = maxTango(left);
            node->left = parent->right;
            if (parent->right) parent->right->parent = node;
            parent->right = node;
            left->parent = node->parent;
            node->parent = parent;
            node->info.red = true;
            node->info.black_height = 0;
            left->parent = initParent;
            if (initParent == nullptr) {
                this->root = left;
            } else if (leftChild) {
                initParent->left = left;
            } else {
                initParent->right = left;
            }
            left->info.marked = true;
            updateMinMaxDepthUp(node);
            if (node->parent->info.red) left = fixupMergeTango(node);
            left->info.marked = marked;
            return left;
        } else if (node->left->info.black_height == node->right->info.black_height) {
            node->info.red = false;
            node->info.black_height = node->left->info.black_height + 1;
            node->info.marked = marked;
            updateMinMaxDepth(node);
            return node;
        } else if (node->left->info.black_height < node->right->info.black_height) {
            return concatenate_right(node->left, node, node->right, initParent, leftChild, marked);
        } else {
            return concatenate_left(node->left, node, node->right, initParent, leftChild, marked);
        }
    }

    void split(TangoNode<K, V> *root, TangoNode<K, V> *pivot) {
        auto parent = root->parent;
        bool leftChild = parent && parent->left == root;
        bool marked = root->info.marked;
        root->info.marked = false;
        if (root == pivot) {
            if (pivot->left && pivot->left->info.red) {
                pivot->left->info.red = false;
                pivot->left->info.black_height++;
            }
            if (pivot->right && pivot->right->info.red) {
                pivot->right->info.red = false;
                pivot->right->info.black_height++;
            }
            pivot->info.marked = marked;
            return;
        }
        if (pivot->key < root->key) {
            split(root->left, pivot);
            root->left = pivot->right;
            if (pivot->right) pivot->right->parent = root;
            if (root->right && root->right->info.red) {
                root->right->info.red = false;
                root->right->info.black_height++;
            }
            auto R = concatenate(root);
            pivot->right = R;
            R->parent = pivot;
        } else {
            split(root->right, pivot);
            root->right = pivot->left;
            if (pivot->left) pivot->left->parent = root;
            if (root->left && root->left->info.red) {
                root->left->info.red = false;
                root->left->info.black_height++;
            }
            auto L = concatenate(root);
            pivot->left = L;
            L->parent = pivot;
        }
        pivot->info.marked = marked;
        pivot->parent = parent;
        if (parent == nullptr) {
            this->root = pivot;
        } else if (leftChild) {
            parent->left = pivot;
        } else {
            parent->right = pivot;
        }
    }

    void initializeTango(TangoNode<K, V> *node, int depth) {
        if (node == nullptr) return;
        node->info.marked = true;
        node->info.red = false;
        node->info.black_height = 1;
        node->info.reference_depth = node->info.min_depth = node->info.max_depth = depth;
        initializeTango(node->left, depth + 1);
        initializeTango(node->right, depth + 1);
    }

    void initializeTango() { initializeTango(this->root, 0); }

    void updateMinMaxDepth(TangoNode<K, V> *node) {
        int min_depth = node->info.reference_depth;
        int max_depth = min_depth;
        auto left = node->left, right = node->right;
        if (!isChildNil(left)) {
            min_depth = min(min_depth, left->info.min_depth);
            max_depth = max(max_depth, left->info.max_depth);
        }
        if (!isChildNil(right)) {
            min_depth = min(min_depth, right->info.min_depth);
            max_depth = max(max_depth, right->info.max_depth);
        }
        node->info.min_depth = min_depth;
        node->info.max_depth = max_depth;
    }

    void updateMinMaxDepthUp(TangoNode<K, V> *node) {
        updateMinMaxDepth(node);
        while (node->parent != nullptr && !node->info.marked) {
            node = node->parent;
            updateMinMaxDepth(node);
        }
    }

    TangoNode<K, V> *minWithDepth(TangoNode<K, V> *root, int depth) {
        if (root->info.max_depth <= depth) return nullptr;
        while (true) {
            if (!isChildNil(root->left) && root->left->info.max_depth > depth) {
                root = root->left;
            } else if (root->info.reference_depth > depth) {
                return root;
            } else {
                root = root->right;
            }
        }
    }

    TangoNode<K, V> *maxWithDepth(TangoNode<K, V> *root, int depth) {
        if (root->info.max_depth <= depth) return nullptr;
        while (true) {
            if (!isChildNil(root->right) && root->right->info.max_depth > depth) {
                root = root->right;
            } else if (root->info.reference_depth > depth) {
                return root;
            } else {
                root = root->left;
            }
        }
    }

    TangoNode<K, V> *pred(TangoNode<K, V> *node) {
        if (!isChildNil(node->left)) {
            node = node->left;
            while (!isChildNil(node->right)) {
                node = node->right;
            }
            return node;
        }
        while (!node->info.marked && node->parent && node->parent->left == node) {
            node = node->parent;
        }
        if (!node->info.marked && node->parent) {
            return node->parent;
        }
        return nullptr;
    }

    TangoNode<K, V> *succ(TangoNode<K, V> *node) {
        if (!isChildNil(node->right)) {
            node = node->right;
            while (!isChildNil(node->left)) {
                node = node->left;
            }
            return node;
        }
        while (!node->info.marked && node->parent && node->parent->right == node) {
            node = node->parent;
        }
        if (!node->info.marked && node->parent) {
            return node->parent;
        }
        return nullptr;
    }

    TangoNode<K, V> *cut(TangoNode<K, V> *root, int depth) {
        if (root->info.max_depth <= depth) return root;
        if (root->info.min_depth > depth) return nullptr;
        auto l = minWithDepth(root, depth);
        TangoNode<K, V> *L = pred(l);
        auto r = maxWithDepth(root, depth);
        TangoNode<K, V> *R = succ(r);
        if (L && R) {
            split(root, L);
            split(L->right, R);
            auto B = R->left;
            B->info.marked = true;
            concatenate(R);
            return concatenate(L);
        } else if (L) {
            split(root, L);
            auto B = L->right;
            B->info.marked = true;
            return concatenate(L);
        } else if (R) {
            split(root, R);
            auto B = R->left;
            B->info.marked = true;
            return concatenate(R);
        }
        throw;
    }

    TangoNode<K, V> *search(TangoNode<K, V> *root, K key) {
        if (root == nullptr) throw;
        auto current = root;
        while (true) {
            if (current->key == key) return current;
            if (current->key > key) {
                if (isChildNil(current->left)) {
                    return current;
                }
                current = current->left;
            } else {
                if (isChildNil(current->right)) {
                    return current;
                }
                current = current->right;
            }
        }
        return current;
    }

    TangoNode<K, V> *minTango(TangoNode<K, V> *node) {
        while (!isChildNil(node->left)) {
            node = node->left;
        }
        return node;
    }

    TangoNode<K, V> *maxTango(TangoNode<K, V> *node) {
        while (!isChildNil(node->right)) {
            node = node->right;
        }
        return node;
    }

    TangoNode<K, V> *join(TangoNode<K, V> *root, TangoNode<K, V> *other) {
        if (root->info.reference_depth > other->info.reference_depth) {
            swap(root, other);
        }
        auto LR = search(root, other->key);
        TangoNode<K, V> *L = nullptr, *R = nullptr;
        if (LR->right == other) {
            L = LR;
            R = succ(L);
        } else {
            R = LR;
            L = pred(R);
        }
        if (L && R) {
            split(root, L);
            split(L->right, R);
            R->left->info.marked = false;
            concatenate(R);
            return concatenate(L);
        } else if (L) {
            split(root, L);
            auto right = L->right;
            right->info.marked = false;
            return concatenate(L);
        } else if (R) {
            split(root, R);
            auto left = R->left;
            left->info.marked = false;
            return concatenate(R);
        }
        throw;
    }

   public:
    TangoTree() : BST<K, V, TangoNodeInfo>() {}

    void checkIntegrity() {
        if (this->root) {
            assert(this->root->parent == nullptr);
            checkIntegrity(this->root);
        }
    }

    void print(TangoNode<K, V> *node) {
        if (node == nullptr) {
            cout << "<<Empty>>";
            return;
        }
        auto root = node->info.marked ? "root " : "";
        auto red = node->info.red ? "red " : "black ";
        cout << "<< " << root << red << node->key << ":" << node->val << " d"
             << node->info.reference_depth << " b" << node->info.black_height << " ["
             << node->info.min_depth << ", " << node->info.max_depth << "] c:";
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
            cout << node->key << (node->info.red ? "r" : "") << " " << node->right->key
                 << (node->right->info.red ? "r" : "") << endl;
        }
        if (node->left) {
            cout << node->key << (node->info.red ? "r" : "") << " " << node->left->key
                 << (node->left->info.red ? "r" : "") << endl;
        }
        if (!isChildNil(node->left)) printGraph(node->left);
        if (!isChildNil(node->right)) printGraph(node->right);
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

    void lock() {
        locked = true;
        initializeTango();
    }

    TangoNode<K, V> *find(K key) {
        if (!this->locked) throw;
        auto root = this->root;
        if (root == nullptr) throw;
        auto current = root;
        vector<TangoNode<K, V> *> nodesToBeMerged;
        while (true) {
            current = search(current, key);
            if (current->key == key) {
                if (current->left && current->left->info.marked) {
                    nodesToBeMerged.push_back(current->left);
                }
                break;
            }
            if (current->key > key) {
                if (current->left == nullptr) throw;
                auto left = current->left;
                nodesToBeMerged.push_back(left);
                current = left;
            } else {
                if (current->right == nullptr) throw;
                auto right = current->right;
                nodesToBeMerged.push_back(right);
                current = right;
            }
        }
        for (auto node : nodesToBeMerged) {
            root = cut(root, node->info.min_depth - 1);
            root = join(root, node);
        }
        return current;
    }
};