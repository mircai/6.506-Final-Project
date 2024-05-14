#include "avl.h"
#include <vector>
#include <set>

template <class T>
struct CTree {
    using TT = std::vector<T>;
    TT prefix;
    int b;
    AVL<T, TT> *avl; // = new AVL<T, TT>();

    // copy constructor
    CTree(CTree &other) {
        prefix = other.prefix;
        b = other.b;
        avl = other.avl;
    }

    CTree(TT vals, int b) : b(b) {
        int n = vals.size();
        avl = new AVL<T, TT>();
        std::vector<bool> heads(n);
        // insert heads
        for (int i = 0; i < n; i ++) {
            T &val = vals[i];
            auto h = std::hash<T>{}(val);
            if (h % b == 0) {
                avl->root = avl->_insert(avl->root, val, {}, true);
                heads[i] = true;
            }
        }
        // insert tails
        // no need for initialization to be purely functional
        for (int i = 0; i < n; i ++) {
            if (!heads[i]) {
                T &val = vals[i];
                auto p = avl->find_lesser(val);
                if (p == nullptr) {
                    prefix.push_back(val);
                } else {
                    TT &tail = p->value;
                    tail.push_back(val);
                }
            }
        }
    }

    // for testing w/set heads
    CTree(std::set<T> heads, TT elms) {
        int n = elms.size();
        avl = new AVL<T, TT>();
        // insert heads
        for (auto head: heads) {
            for (auto elm: elms) {
                if (elm == head) {
                    avl = avl->insert(head, {});
                    break;
                }
            }
        }
        // insert tails
        // no need for initialization to be purely functional
        for (auto val: elms) {
            if (!heads.count(val)) {
                auto p = avl->find_lesser(val);
                if (p == nullptr) {
                    prefix.push_back(val);
                } else {
                    TT &tail = p->value;
                    tail.push_back(val);
                }
            }
        }
    }

    CTree(int *edges, int64_t n_edges, int b) {
        avl = new AVL<T, TT>();
        std::vector<bool> heads(n_edges);
        // insert heads
        for (int i = 0; i < n_edges; i++) {
            int val = edges[i];
            auto h = std::hash<T>{}(val);
            if (h % b == 0) {
                avl = avl->insert(val, {});
                heads[i] = true;
            }
            else { heads[i] = false; }
        }
        // insert tails
        // no need for initialization to be purely functional
        for (int i = 0; i < n_edges; i++) {
            int val = edges[i];
            if (!heads[i]) {
                auto p = avl->find_lesser(val);
                if (p == nullptr) {
                    prefix.push_back(val);
                } else {
                    TT &tail = p->value;
                    tail.push_back(val);
                }
            }
        }
    }

    /*
    TODO: add insert function

    note that initialization cannot use the function since all
    heads need to be inserted before anything else
    */
//     CTree batch_insert(TT elms) {
//     return _union(CTree(elms, this->b));
//     }

//     // union of two ctrees
//     CTree _union(CTree other) {

//     }
    pair<TT, TT> splitChunk(TT &chunk, T &val) {
        TT le, g;
        for (T &e:chunk) {
            (e <= val ? le : g).push_back(e);
        }
        return {le, g};
    }
    CTree* _insert_mutable(T val) {
        auto h = std::hash<T>{}(val);
        // pointer to next smallest head
        auto p = avl->find_lesser(val);
        if (h % b == 0) {
            if (p == nullptr) {
                auto [le, g] = splitChunk(prefix, val);
                prefix = le;
                avl->root = avl->_insert(avl->root, val, g, true);
            } else {
                auto [le, g] = splitChunk(p->value, val);
                avl->root = avl->_insert(avl->root, p->key, le, true);
                avl->root = avl->_insert(avl->root, val, g, true);
            }
        } else {
            if (p == nullptr) {
                prefix.push_back(val);
            } else {
                TT &new_tail = p->value; // '=' copies vector
                new_tail.push_back(val);
                // avl->root = avl->_insert(avl->root, p->key, new_tail, true);
            }
        }
        return this;
    }
    CTree* insert(T val) {
        auto res = new CTree<T>(*this);
        auto h = std::hash<T>{}(val);
        // pointer to next smallest head
        auto &prefix = res->prefix;
        auto &avl = res->avl;
        auto p = avl->find_lesser(val);
        if (h % b == 0) {
            if (p == nullptr) {
                auto [le, g] = splitChunk(prefix, val);
                prefix = le;
                avl = avl->insert(val, g);
            } else {
                auto [le, g] = splitChunk(p->value, val);
                avl = avl->insert(p->key, le);
                avl->root = avl->_insert(avl->root, val, g, true);
            }
        } else {
            if (p == nullptr) {
                prefix.push_back(val);
            } else {
                TT new_tail = p->value; // '=' copies vector
                new_tail.push_back(val);
                avl = avl->insert(p->key, new_tail);
            }
        }
        return res;
    }

};
