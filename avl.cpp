#include "avl.h"
using namespace std;

int main() {
    AVL<int> tree;
    for(int i = 1; i < 128; i += 2){
    // for(int i = 127; i > 0; i --){
        tree.insert(i);
    }
    tree.root->print();
    cout << (*tree.find_lesser(200)) << endl;
    cout << (*tree.find_lesser(100)) << endl;
    cout << (*tree.find_greater(-1)) << endl;
    cout << "done" << endl;
}