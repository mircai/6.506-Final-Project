#pragma once

#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
using namespace std;

template <class T>
struct Node {
    T value;
    Node *left;
    Node *right;
    uint32_t h = 1;

    Node(T &value, Node *left, Node *right) {
        this->value = value;
        this->left = left;
        this->right = right;
        this->h = 1 + std::max(Node::height(left), Node::height(right));
    }

    Node(Node *n) {
        this->value = n->value;
        this->left = n->left;
        this->right = n->right;
        this->h = n->h;
    }


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
        _print(this, 0);
    }

    static void _print(Node<T>* p, int indent) {
        if(p != nullptr) {
            if(p->right) {
                _print(p->right, indent+4);
            }
            if (indent) {
                std::cout << std::setw(indent) << ' ';
            }
            if (p->right) std::cout<<" /\n" << std::setw(indent) << ' ';
            std::cout<< p->value << "\n ";
            if(p->left) {
                std::cout << std::setw(indent) << ' ' <<" \\\n";
                _print(p->left, indent+4);
            }
        }
    }
};

template <class T>
struct AVL {
    Node<T> *root = nullptr;

    inline AVL* insert(T val) {
        AVL *avl = new AVL;
        avl->root = _insert(this->root, val);
        return avl;
    }

    /*
    returns a pointer to the largest element < val
    if no such element exists, returns nullptr
    */
    inline T* find_lesser(T val) {
        return _find_lesser(this->root, val);
    }

    /*
    returns a pointer to the largest element > val
    if no such element exists, returns nullptr
    */
    inline T* find_greater(T val) {
        return _find_greater(this->root, val);
    }

private:
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

    /*
    insert `n` at subtree with root `r`
    */
    static Node<T>* _insert(Node<T> *r, T val) {
        if (r == nullptr) {
            return new Node<T>(val, nullptr, nullptr);
        }
        r = new Node<T>(r);
        // assumes no duplicates since the graph has no
        // repeat edges
        if (val < r->value) {
            r->left = _insert(r->left, val);
        } else {
            r->right = _insert(r->right, val);
        }
        Node<T>::update_height(r);
        int skew = Node<T>::get_skew(r);
        if (skew > 1) {
            if (val > r->left->value) {
                r->left = left(r->left);
            }
            return right(r);
        }
        if (skew < -1) {
            if (val < r->right->value) {
                r->right = right(r->right);
            }
            return left(r);
        }
        return r;
    }

    /*
    see find_lesser
    */
    static T* _find_lesser(Node<T> *r, T val) {
        if (r == nullptr) return nullptr;
        if (r->value < val) {
            T *right_res = _find_lesser(r->right, val);
            return right_res == nullptr ? &r->value : right_res;
        } else {
            return _find_lesser(r->left, val);
        }
    }

    /*
    see find_greater
    */
    static T* _find_greater(Node<T> *r, T val) {
        if (r == nullptr) return nullptr;
        if (r->value > val) {
            T *left_res = _find_greater(r->left, val);
            return left_res == nullptr ? &r->value : left_res;
        } else {
            return _find_greater(r->right, val);
        }
    }
};
