#include "avl.h"
#include <vector>

template <class T>
struct CTree {
    using TT = std::vector<T>;
    TT prefix;
    AVL<TT> *avl;

    CTree(TT vals, int b) {
        int n = vals.size();
        this->avl = new AVL<TT>();
        std::vector<bool> heads(n);
        // insert heads
        for (int i = 0; i < n; i ++) {
            T &val = vals[i];
            auto h = std::hash<T>{}(val);
            if (h % b == 0) {
                avl = avl->insert({val});
                heads[i] = true;
            }
        }
        // insert tails
        // TODO: inserting tails should also be purely functional (return a new copy)
        for (int i = 0; i < n; i ++) {
            if (!heads[i]) {
                T &val = vals[i];
                TT *head = avl->find_lesser({val});
                (head == nullptr ? &prefix : head)->push_back(val);
            }
        }
    }
    /*
    TODO: add insert function

    note that initialization cannot use the function since all
    heads need to be inserted before anything else
    */
};
