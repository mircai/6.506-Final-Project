#include "ctree.h"
#include <vector>
#include <ctime>  
#include <set>
#include <random>
#include <numeric>
using namespace std;

static mt19937 gen(time(nullptr));
// static default_random_engine generator;
// static uniform_real_distribution<float> distribution(0.0,1.0);

inline int nptr() {
    return -1;
}

inline int sample_size(int step) {
    if (step == -1) return 1;
    if (step == 0) return 25;
    return 10;
    // if (step == 0) return 2;
    // return 3;
}

inline int steps() {
    return 2;
}

inline int num_samples() {
    return 4000;
}

template <class T = uint32_t>
vector<T> get_initial_transits(T seeds_size, T graph_size) {
    vector<T> n_ids;
    for (int i = 0; i < seeds_size; i++) {
        n_ids.push_back(gen() % graph_size);
    }
    return n_ids;
}

template <class T = int, class TT = CTree<T>>
T sample_next(AVL<T, TT> &avl, T transit) {
    CTree<T> t_ctree = avl.get_node(transit)->value;
    vector<T> neighbors = t_ctree.avl->get_all_nodes();
    for (auto prefix: t_ctree.prefix) {
        neighbors.push_back(prefix);
    }
    // cout << transit << ": ";
    // for (auto v: neighbors) cout << v << " ";
    // cout << endl;
    int deg = neighbors.size();
    if (deg == 0) return nptr();
    int idx = gen() % deg;
    // cout << "idx: " << idx << ", deg " << deg << endl;
    return neighbors[idx];
}