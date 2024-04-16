#include "avl.h"
#include <vector>
#include <set>

template <class T>
struct CTree {
    using TT = std::vector<T>;
    TT prefix;
    AVL<T, TT> *avl = new AVL<T, TT>();

    CTree(TT vals, int b) {
        int n = vals.size();
        this->avl = new AVL<T, TT>();
        std::vector<bool> heads(n);
        // insert heads
        for (int i = 0; i < n; i ++) {
            T &val = vals[i];
            auto h = std::hash<T>{}(val);
            if (h % b == 0) {
                avl = avl->insert(val, {});
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
                    T head = p->first;
                    TT tail(p->second);
                    tail.push_back(val);
                    avl = avl->insert(head, tail);
                }
            }
        }
    }

    // for testing w/set heads
    CTree(std::set<T> heads, TT elms) {
        int n = elms.size();
        this->avl = new AVL<T, TT>();
        // insert heads
        for (auto head: heads) {
            avl = avl->insert(head, {});
        }
        // insert tails
        // no need for initialization to be purely functional
        for (auto val: elms) {
            if (!heads.count(val)) {
                auto p = avl->find_lesser(val);
                if (p == nullptr) {
                    prefix.push_back(val);
                } else {
                    T head = p->first;
                    TT tail(p->second);
                    tail.push_back(val);
                    avl = avl->insert(head, tail);
                }
            }
        }
    }

    /*
    TODO: add insert function

    note that initialization cannot use the function since all
    heads need to be inserted before anything else
    */
//    inline insert()

};
