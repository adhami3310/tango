
#include <iostream>
#include <sstream>      // std::stringstream
#include <vector>
#include <assert.h>
#include <set>

#include "bst.hpp"
using namespace std;

enum SwitchDirection {
    None,
    LeftToRight,
    RightToLeft
};

struct MultiSplayNodeInfo {
    // In addition, each node
    // v stores the minimum depth of all of the nodes in
    // splaySubtree(v) in its minDepth field (splaySubtree(v)
    // contains all of the nodes in the same splay tree as v
    // that have v as an ancestor, including v)
    int min_depth = -1;

    int reference_depth = -1;
    bool marked = false; // is root of an auxiliary tree

    // red-black tree maintained for initial inserts/removals
    bool red = true;
};

template <typename K, typename V>
using MultiSplayNode = BSTNode<K, V, MultiSplayNodeInfo>;

template <typename K, typename V>
class MultiSplayTree : public BST<K, V, MultiSplayNodeInfo> {
private:
    bool locked;

    bool isChildNil(MultiSplayNode<K, V> *node) {
        return node == nullptr || node->info.marked;
    }

    void updateMinDepth(MultiSplayNode<K, V> *node) {
        int min_depth = node->info.reference_depth;
        auto left = node->left, right = node->right;
        if (!isChildNil(left)) {
            min_depth = min(min_depth, left->info.min_depth);
        }
        if (!isChildNil(right)) {
            min_depth = min(min_depth, right->info.min_depth);
        }
        node->info.min_depth = min_depth;
    }

    void left_rotate(MultiSplayNode<K, V> *node) {
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
        updateMinDepth(node);
        updateMinDepth(child);
    }

    void right_rotate(MultiSplayNode<K, V> *node) {
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
        updateMinDepth(node);
        updateMinDepth(child);
    }
    
    
    void fixupInsert(MultiSplayNode<K, V> *node) {
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

    
    void resolveDoubleBlack(MultiSplayNode<K, V> *node) {
        auto parent = node->parent;
        MultiSplayNode<K, V> *sibling = nullptr;
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

    void removeNode(MultiSplayNode<K, V> *node) {
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

    MultiSplayNode<K, V> *getRemoveNode(MultiSplayNode<K, V> *node) {
        if (node == nullptr) throw;
        if (node->left == nullptr || node->right == nullptr) {
            return node;
        }
        auto prev = node->pred();
        this->swapValues(prev, node);
        return getRemoveNode(prev);
    }

    
    int sizeAuxiliary(MultiSplayNode<K, V> *node) {
        int result = 1;
        if (!isChildNil(node->left)) {
            result += sizeAuxiliary(node->left);
        }
        if (!isChildNil(node->right)) {
            result += sizeAuxiliary(node->right);
        }
        return result;
    }

    set<K> depthsAuxiliary(MultiSplayNode<K, V> *node) {
        set<K> result = {node->info.reference_depth};
        if (!isChildNil(node->left)) {
            for (auto a: depthsAuxiliary(node->left)) {
                result.insert(a);
            }
        }
        if (!isChildNil(node->right)) {
            for (auto a: depthsAuxiliary(node->right)) {
                result.insert(a);
            }
        }
        return result;
    }

    void checkIntegrity(MultiSplayNode<K, V> *node) {
        if (node == nullptr) return;
        assert(node->info.reference_depth >= node->info.min_depth);
        auto parent = node->parent;
        if (parent) {
            if (!node->info.marked) {
                assert(node->info.min_depth >= parent->info.min_depth);
            }
        } else {
            assert(this->root == node);
            assert(node->info.marked);
        }

        assert(depthsAuxiliary(node).size() == sizeAuxiliary(node));
        
        if (node->left) {
            assert(node->key > node->left->max()->key);
            assert(node->left->parent == node);
        }
        if (node->right) {
            assert(node->key < node->right->min()->key);
            assert(node->right->parent == node);
        }
        
        if (isChildNil(node->left) && isChildNil(node->right)) {
            assert(node->info.reference_depth == node->info.min_depth);
        } else if (isChildNil(node->right)) {
            assert(min(node->info.reference_depth, node->left->info.min_depth) == node->info.min_depth);
        } else if (isChildNil(node->left)) {
            assert(min(node->info.reference_depth, node->right->info.min_depth) == node->info.min_depth);
        } else {
            assert(min(min(node->info.reference_depth, node->right->info.min_depth), node->left->info.min_depth) == node->info.min_depth);
        }
        checkIntegrity(node->left);
        checkIntegrity(node->right);
    }

    

    void initializeMultiSplay(MultiSplayNode<K, V> *node, int depth) {
        if (node == nullptr) return;
        node->info.marked = true;
        node->info.red = false;
        node->info.reference_depth = node->info.min_depth = depth;
        initializeMultiSplay(node->left, depth + 1);
        initializeMultiSplay(node->right, depth + 1);
    }

    void initializeMultiSplay() {
        initializeMultiSplay(this->root, 0); 
    }

    
    MultiSplayNode<K, V> *pred(MultiSplayNode<K, V> *node) {
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

    MultiSplayNode<K, V> *succ(MultiSplayNode<K, V> *node) {
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

    MultiSplayNode<K, V> *search(MultiSplayNode<K, V> *root, K key, MultiSplayNode<K, V> *&deepestNode, SwitchDirection &dir) {
        if (root == nullptr) throw;
        auto current = root;

        deepestNode = nullptr;
        dir = None;

        while (true) {
            if (current->key == key) return current;
            if (current->key > key) {
                if (deepestNode == nullptr || dir == RightToLeft || current->info.reference_depth > deepestNode->info.reference_depth) {
                    deepestNode = current;
                    dir = RightToLeft;
                }
                if (isChildNil(current->left)) {
                    return current;
                }
                current = current->left;
            } else {
                if (deepestNode == nullptr || dir == LeftToRight || current->info.reference_depth > deepestNode->info.reference_depth) {
                    deepestNode = current;
                    dir = LeftToRight;
                }
                if (isChildNil(current->right)) {
                    return current;
                }
                current = current->right;
            }
        }
        return current;
    }
    

    // Returns max node with depth < depth
    MultiSplayNode<K, V> *maxWithDepthLessThan(MultiSplayNode<K, V> *root, int depth) {
        if (root == nullptr || root->info.min_depth >= depth) return nullptr;

        while (root != nullptr) {
            if (!isChildNil(root->right) && root->right->info.min_depth < depth) {
                root = root->right;
            } else if (root->info.reference_depth < depth) {
                return root;
            } else {
                root = root->left;
            }
        }

        return nullptr;
    }

    // Returns min node with depth < depth
    MultiSplayNode<K, V> *minWithDepthLessThan(MultiSplayNode<K, V> *root, int depth) {
        if (root == nullptr || root->info.min_depth >= depth) return nullptr;

        while (root != nullptr) {
            if (!isChildNil(root->left) && root->left->info.min_depth < depth) {
                root = root->left;
            } else if (root->info.reference_depth < depth) {
                return root;
            } else {
                root = root->right;
            }
        }

        return nullptr;
    }

    void rotate(MultiSplayNode<K, V> *node) {
        auto par = node->parent;
        if (par == nullptr || node->info.marked) return;

        node->rotate();

        swap(node->info.marked, par->info.marked);
        updateMinDepth(par);
        updateMinDepth(node);
    }

    // splay step unless root is our parent
    // return true if still needs more splaying
    bool splayStep(MultiSplayNode<K, V>* node, MultiSplayNode<K, V>* root) {
        if (node == root || node->info.marked) return false;

        auto par = node->parent;
        if (par == root || par == nullptr) return false;

        if (par->parent == root || par->info.marked) {
            rotate(node);
            return false;
        }

        auto grandparent = par->parent;
        if (node->is_left_child() == par->is_left_child()) {
            // zig-zig and zag-zag
            rotate(par);
            rotate(node);
        } else {
            // zig-zag and zag-zig
            rotate(node);
            rotate(node);
        }

        if (node->info.marked) {
            // swap(node->info.marked, grandparent->info.marked);
            return false;
        }
        return true;
    }

    // splay until directly beneath root
    void splayUpto(MultiSplayNode<K, V>* node, MultiSplayNode<K, V>* root) {
        if (node == root) return;

        while(splayStep(node, root)) { }

        if (node->parent == nullptr) {
            this->root = node;
            node->info.marked = true;
        }
    }

    // splay until at top of splay tree
    void splay(MultiSplayNode<K, V>* node) {
        splayUpto(node, nullptr);
    }

    void switchLeftToRight(MultiSplayNode<K, V>* node) {
        if (node == nullptr) throw;

        splay(node);

        // L, R are left, right sections
        // l < L < node < R < r

        auto l = maxWithDepthLessThan(node->left, node->info.reference_depth);
        auto r = minWithDepthLessThan(node->right, node->info.reference_depth);

        if (l) {
            splayUpto(l, node);
            if (l->right) {
                l->right->info.marked = true;
            }
            updateMinDepth(l);
        } else if (node->left) {
            node->left->info.marked = true;
        }

        if (r) {
            splayUpto(r, node);
            if (r->left) {
                r->left->info.marked = false;
            }
            updateMinDepth(r);
        } else if (node->right) {
            node->right->info.marked = false;
        }

        updateMinDepth(node);
    }
    
    void switchRightToLeft(MultiSplayNode<K, V>* node) {
        if (node == nullptr) throw;

        splay(node);

        // L, R are left, right sections
        // l < L < node < R < r

        auto l = maxWithDepthLessThan(node->left, node->info.reference_depth);
        auto r = minWithDepthLessThan(node->right, node->info.reference_depth);

        if (r) {
            splayUpto(r, node);
            if (r->left) {
                r->left->info.marked = true;
            }
            updateMinDepth(r);
        } else if (node->right) {
            node->right->info.marked = true;
        }

        if (l) {
            splayUpto(l, node);
            if (l->right) {
                l->right->info.marked = false;
            }
            updateMinDepth(l);
        } else if (node->left) {
            node->left->info.marked = false;
        }

        updateMinDepth(node);

    }

public:
    MultiSplayTree() : BST<K, V, MultiSplayNodeInfo>(), locked(false) {}

    void lock() {
        locked = true;
        initializeMultiSplay();
    }

    void checkIntegrity() {
        if (this->root) {
            assert(this->root->parent == nullptr);
            checkIntegrity(this->root);
        }
    }

    void print(MultiSplayNode<K, V> *node) {
        if (node == nullptr) {
            cout << "<<Empty>>";
            return;
        }
        cout << "<< " << node->key << ":" << node->val << ", mark:" << node->info.marked << ", c:";
        print(node->left);
        print(node->right);
        cout << ">>";
    }

    void print() {
        cout << "{{";
        print(this->root);
        cout << "}}" << endl;
    }

    void pretty_print(MultiSplayNode<K, V> *node, string prefix, string sep, bool info) {
        if (node == nullptr) return;

        string n_prefix = prefix + sep;

        if (node->right) {
            string r_prefix = n_prefix;
            if (node->is_left_child()) {
                r_prefix = prefix + "|" + sep.substr(0, sep.length()-1);
            }
            pretty_print(node->right, r_prefix, sep, info);
            cout << r_prefix << "/" << endl;
        }
        string c_prefix = prefix;
        if (node->info.marked) {
            c_prefix = prefix + "*";
        }
        string c_suffix = "";
        if (info) {
            stringstream ss;
            ss << " {marked: " << node->info.marked << ", ref_depth: " << node->info.reference_depth << ", min_depth: " << node->info.min_depth << "}";
            c_suffix = ss.str();
        }
        cout << c_prefix << node->key << c_suffix << endl;
        if (node->left) {
            string l_prefix = n_prefix;
            if (!node->is_left_child() && node->parent != nullptr) {
                l_prefix = prefix + "|" + sep.substr(0, sep.length()-1);
            }
            cout << l_prefix << "\\" << endl;
            pretty_print(node->left, l_prefix, sep, info);
        }
    }

    void pretty_print(string sep, bool info) {
        pretty_print(this->root, "", sep, info);
    }

    void insert(K key, V val) {
        if (locked) throw;
        auto node = new MultiSplayNode(key, val, MultiSplayNodeInfo());
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


    MultiSplayNode<K, V>* find(K key) {
        if (!this->locked) throw;
        auto root = this->root;
        if (root == nullptr) throw;
        auto current = root;
        vector<pair<MultiSplayNode<K, V> *, SwitchDirection>> nodesToBeSwitched;

        while (true) {
            MultiSplayNode<K, V>* switchNode;
            SwitchDirection switchDir;

            current = search(current, key, switchNode, switchDir);

            if (!current) throw;
            
            if (current->key == key) {
                nodesToBeSwitched.push_back(make_pair(current, None));
                break;
            }

            if (switchDir != None) {
                nodesToBeSwitched.push_back(make_pair(switchNode, switchDir));
            }

            if (current->key > key) {
                if (current->left == nullptr) throw;
                current = current->left;
            } else {
                if (current->right == nullptr) throw;
                current = current->right;
            }
        }

        for (pair<MultiSplayNode<K, V> *, SwitchDirection> switchPair : nodesToBeSwitched) {
            MultiSplayNode<K, V> *switchNode = switchPair.first;
            SwitchDirection switchDir = switchPair.second;

            if (switchDir == None) {
                splay(switchNode);
            } else if (switchDir == LeftToRight) {
                switchLeftToRight(switchNode);
            } else if (switchDir == RightToLeft) {
                switchRightToLeft(switchNode);
            } else {
                throw;
            }
        }
        
        return current;
    }
};