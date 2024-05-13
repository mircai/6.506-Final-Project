#include "avl.h"
using namespace std;

int main() {
    AVL<int, int> tree;
    AVL<int, int> tree32;
    for(int i = 1; i < 128; i ++){
    // for(int i = 127; i > 0; i --){
        if (i == 32) tree32 = tree;
        tree = *tree.insert(i, 1);
    }
    tree.root->print();
    cout << "=============================================" << endl;
    tree32.root->print();
    cout << "=============================================" << endl;
    cout << tree.find_lesser(200)->key << endl;
    cout << tree32.find_lesser(200)->key << endl;
    cout << "done" << endl;
}