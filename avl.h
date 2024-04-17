#pragma once

#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
using namespace std;

template <class K, class V>
struct Node {
    using KV = std::pair<K*, V*>;
    K key;
    V value;
    Node *left;
    Node *right;
    uint32_t h = 1;

    Node(const K &key, const V &value, Node *left, Node *right) :
        key(key),
        value(value),
        left(left),
        right(right) {
        this->h = 1 + std::max(Node::height(left), Node::height(right));
    }

    Node(Node *n) :
        key(n->key),
        value(n->value),
        left(n->left),
        right(n->right),
        h(n->h) {}

    inline KV* getKV() {
        return new KV(&this->key, &this->value);
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
    using KV = std::pair<K*, V*>;
    Node<K, V> *root = nullptr;

    inline AVL* insert(K key, V val) {
        AVL *avl = new AVL;
        avl->root = _insert(this->root, key, val, false);
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

    inline Node<K,V>* get_node(K key) {
        return _find(this->root, key);
    }

    inline vector<K> get_all_nodes() {
        vector<K> all_nodes;
        _get_all_nodes(this->root, all_nodes);
        return all_nodes;
    }

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
    static Node<K, V>* _insert(Node<K, V> *r, K key, V val, bool mut) {
        if (r == nullptr) {
            return new Node<K, V>(key, val, nullptr, nullptr);
        }
        if (!mut) r = new Node<K, V>(r);
        if (key == r->key) {
            r->value = val;
        } else
        if (key < r->key) {
            r->left = _insert(r->left, key, val, mut);
        } else {
            r->right = _insert(r->right, key, val, mut);
        }
        Node<K, V>::update_height(r);
        int skew = Node<K, V>::get_skew(r);
        if (skew > 1) {
            if (key > r->left->key) {
                r->left = left(r->left);
            }
            return right(r);
        }
        if (skew < -1) {
            if (key < r->right->key) {
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

    static Node<K,V>* _find(Node<K, V> *r, K target) {
        if (r == nullptr) return nullptr;
        if (r->key == target) return r;
        if (r->key > target) {
            return _find(r->left, target);
        }
        else {
            return _find(r->right, target);
        }
    }

    static inline void _get_all_nodes(Node<K, V> *a, vector<K> &curr_nodes) {
        if (a == nullptr) return;
        curr_nodes.push_back(a->key);
        for (auto val: a->value) {
            curr_nodes.push_back(val);
        }
        _get_all_nodes(a->left, curr_nodes);
        _get_all_nodes(a->right, curr_nodes);
        return;
    }
};
