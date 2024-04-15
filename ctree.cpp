#include "avl.h"
#include <vector>
using namespace std;

template <class T>
struct CTree {
    using TT = vector<T>;
    TT prefix;
    AVL<TT> *avl;

    CTree(TT vals, int b) {
        int n = vals.size();
        this->avl = new AVL<TT>();
        vector<bool> heads(n);
        // insert heads
        for (int i = 0; i < n; i ++) {
            T &val = vals[i];
            auto h = std::hash<T>{}(val);
            if (h % b == 0) {
                avl->insert({val});
                heads[i] = true;
            }
        }
        // insert tails
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

#define print(v) {for(auto&i:(v))cout<<i<<" ";cout<<endl;}

int main() {
    vector<int> vals = {1,2,3,4,5,6,7,8,9,10};
    CTree t(vals, 3);
    auto avl = t.avl;
    print(avl->root->value);
    print(avl->root->left->value);
    print(avl->root->right->value);
    print(t.prefix);
}