#pragma once

#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
using namespace std;

template <class K, class V>
struct Node {
    using KV = std::pair<K, V>;
    K key;
    V value;
    Node *left;
    Node *right;
    uint32_t h = 1;

    Node(K &key, V &value, Node *left, Node *right) {
        this->key = key;
        this->value = value;
        this->left = left;
        this->right = right;
        this->h = 1 + std::max(Node::height(left), Node::height(right));
    }

    Node(Node *n) {
        this->key = n->key;
        this->value = n->value;
        this->left = n->left;
        this->right = n->right;
        this->h = n->h;
    }

    inline KV* getKV() {
        return new KV(this->key, this->value);
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

    static void _print(Node *p, int indent) {
        if(p != nullptr) {
            if(p->right) {
                _print(p->right, indent+4);
            }
            if (indent) {
                std::cout << std::setw(indent) << ' ';
            }
            if (p->right) std::cout<<" /\n" << std::setw(indent) << ' ';
            std::cout<< p->key << "\n ";
            if(p->left) {
                std::cout << std::setw(indent) << ' ' <<" \\\n";
                _print(p->left, indent+4);
            }
        }
    }
};

template <class K, class V>
struct AVL {
    using KV = std::pair<K, V>;
    Node<K, V> *root = nullptr;

    inline AVL* insert(K key, V val) {
        AVL *avl = new AVL;
        avl->root = _insert(this->root, key, val);
        return avl;
    }

    /*
    returns a pointer to the largest element < key
    if no such element exists, returns nullptr
    */
    inline KV* find_lesser(K key) {
        return _find_lesser(this->root, key);
    }

    /*
    returns a pointer to the largest element > key
    if no such element exists, returns nullptr
    */
    inline KV* find_greater(K key) {
        return _find_greater(this->root, key);
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
    static inline Node<K, V>* left(Node<K, V> *a) {
        auto b = a->right;
        auto c = b->right; // could be nullptr
        auto x = b->left; // could be nullptr
        b->left = a;
        a->right = x;
        Node<K, V>::update_height(a);
        Node<K, V>::update_height(b);
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
    static inline Node<K, V> *right(Node<K, V> *a) {
        auto b = a->left;
        auto c = b->left;
        auto x = b->right; // could be nullptr
        b->right = a;
        a->left = x;
        Node<K, V>::update_height(a);
        Node<K, V>::update_height(b);
        return b;
    }

    /*
    insert `n` at subtree with root `r`
    */
    static Node<K, V>* _insert(Node<K, V> *r, K key, V val) {
        if (r == nullptr) {
            return new Node<K, V>(key, val, nullptr, nullptr);
        }
        r = new Node<K, V>(r);
        if (key == r->key) {
            r->value = val;
        } else
        if (key < r->key) {
            r->left = _insert(r->left, key, val);
        } else {
            r->right = _insert(r->right, key, val);
        }
        Node<K, V>::update_height(r);
        int skew = Node<K, V>::get_skew(r);
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
    static KV* _find_lesser(Node<K, V> *r, K key) {
        if (r == nullptr) return nullptr;
        if (r->key < key) {
            KV *right_res = _find_lesser(r->right, key);
            return right_res == nullptr ? r->getKV() : right_res;
        } else {
            return _find_lesser(r->left, key);
        }
    }

    /*
    see find_greater
    */
    static KV* _find_greater(Node<K, V> *r, K key) {
        if (r == nullptr) return nullptr;
        if (r->key > key) {
            KV *left_res = _find_greater(r->left, key);
            return left_res == nullptr ? r->getKV() : left_res;
        } else {
            return _find_greater(r->right, key);
        }
    }
};
