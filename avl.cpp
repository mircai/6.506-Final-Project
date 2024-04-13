#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// a node storing an element of type T
template <class T>
struct Node {
    T value;
    Node *left;
    Node *right;
    // number of nodes in subtree (including this one)
    uint32_t h = 1;

    static inline uint32_t height(Node *n) {
        return n == nullptr ? 0 : n->h;
    }
    static inline void update_height(Node *n) {
        n->h = 1 + std::max(Node::height(n->left), Node::height(n->right));
    }
    // positive skew = left skewed
    // negative skew = right skewed
    static inline uint32_t get_skew(Node *n) {
        return n == nullptr ? 0 : Node::height(n->left) - Node::height(n->right);
    }

    void print() {
        // if (this->root != nullptr) _print(this->root);
        postorder(this, 0);
    }
    static void _print(Node<T> *r) {
        // if (r->left != nullptr) AVL::_print(r->left);
        // std::cout << r->value << std::endl;
        // if (r->right != nullptr) AVL::_print(r->right);
        vector<Node<T>*> layer = {r};
        while(layer.size() > 0) {
            vector<Node<T>*> next_layer;
            for(auto&n:layer){
                cout << n->value << " ";
                if (n->left != nullptr) next_layer.push_back(n->left);
                if (n->right != nullptr) next_layer.push_back(n->right);
            }
            cout << endl;
            layer = next_layer;
        }
    }

    static void postorder(Node<T>* p, int indent) {
        if(p != nullptr) {
            if(p->right) {
                postorder(p->right, indent+4);
            }
            if (indent) {
                std::cout << std::setw(indent) << ' ';
            }
            if (p->right) std::cout<<" /\n" << std::setw(indent) << ' ';
            std::cout<< p->value << "\n ";
            if(p->left) {
                std::cout << std::setw(indent) << ' ' <<" \\\n";
                postorder(p->left, indent+4);
            }
        }
    }
};

template <class T>
struct AVL {
    Node<T> *root = nullptr;
    /*
    left rotate:
        A          B
        B   ->  A   C
        x C       x
    returns pointer to B
    caller should update child pointers appropriately
    */
    static inline Node<T>* left(Node<T> *a) {
        auto b = a->right;
        auto c = b->right; // could be nullptr
        auto x = b->left; // could be nullptr
        b->left = a;
        a->right = x;
        Node<T>::update_height(a);
        Node<T>::update_height(b);
        return b;
    }

    /*
    right rotate:
        A        B
        B   ->  C   A
        C x         x
    returns pointer to B
    caller should update child pointers appropriately
    */
    static inline Node<T> *right(Node<T> *a) {
        auto b = a->left;
        auto c = b->left;
        auto x = b->right; // could be nullptr
        b->right = a;
        a->left = x;
        Node<T>::update_height(a);
        Node<T>::update_height(b);
        return b;
    }

    inline void insert(T val) {
        this->root = AVL::_insert(this->root, val);
    }
    // insert `n` at subtree with root `r`
    static Node<T>* _insert(Node<T> *r, T val) {
        if (r == nullptr) {
            Node<T> *n = new Node<T>();
            n->value = val;
            return n;
        }
        // assumes no duplicates since the graph has no
        // repeat edges
        if (val < r->value) {
            r->left = AVL::_insert(r->left, val);
        } else {
            r->right = AVL::_insert(r->right, val);
        }
        Node<T>::update_height(r);
        int skew = Node<T>::get_skew(r);
        if (skew > 1) {
            if (val > r->left->value) {
                r->left = AVL::left(r->left);
            }
            return AVL::right(r);
        }
        if (skew < -1) {
            if (val < r->right->value) {
                r->right = AVL::right(r->right);
            }
            return AVL::left(r);
        }
        return r;
    }
};

int main() {
    AVL<int> tree;
    // for(int i = 1; i < 128; i ++){
    for(int i = 127; i > 0; i --){
        tree.insert(i);
    }
    tree.root->print();

    cout << "done" << endl;
}