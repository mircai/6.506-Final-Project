#include "ctree.h"
#include <vector>
using namespace std;

#define print(v) {cout<<((v)->key)<<" "; for(auto&i:((v)->value))cout<<i<<" ";cout<<endl;}

int main() {
    vector<int> vals = {1,2,3,4,5,6,7,8,9,14};
    cout << "hi\n";
    CTree<int> t(vals, 3);
    auto t2 = *t._insert_mutable(0);
    auto t3 = *t2.insert(12);
    auto avl = t.avl;
    auto avl2 = t2.avl;
    auto avl3 = t3.avl;
    // printing
    print(avl->root);
    print(avl->root->left);
    print(avl->root->right);
    print(avl->root->left->left);
    for(auto&i:t.prefix)cout<<i<<" ";cout<<endl;
    cout << "=====================\n";
    print(avl2->root);
    print(avl2->root->left);
    print(avl2->root->right);
    print(avl2->root->left->left);
    for(auto&i:t2.prefix)cout<<i<<" ";cout<<endl;
    cout << "=====================\n";
    print(avl3->root);
    print(avl3->root->left);
    print(avl3->root->right);
    print(avl3->root->left->left);
    print(avl3->root->right->right);
    for(auto&i:t3.prefix)cout<<i<<" ";cout<<endl;
    // print(t.prefix);
}