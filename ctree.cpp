#include "ctree.h"
#include <vector>
using namespace std;

#define print(v) {for(auto&i:(v))cout<<i<<" ";cout<<endl;}

int main() {
    vector<int> vals = {1,2,3,4,5,6,7,8,9,10};
    CTree<int> t(vals, 3);
    auto avl = t.avl;
    print(avl->root->value);
    print(avl->root->left->value);
    print(avl->root->right->value);
    print(t.prefix);
}