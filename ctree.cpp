#include "ctree.h"
#include <vector>
using namespace std;

#define print(v) {cout<<((v)->key)<<" "; for(auto&i:((v)->value))cout<<i<<" ";cout<<endl;}

int main() {
    vector<int> vals = {1,2,3,4,5,6,7,8,9,10};
    cout << "hi\n";
    CTree<int> t(vals, 3);
    auto avl = t.avl;
    print(avl->root);
    print(avl->root->left);
    print(avl->root->right);
    for(auto&i:t.prefix)cout<<i<<" ";cout<<endl;
    // print(t.prefix);
}