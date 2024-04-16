#include "khop.h"
#include "timer.h"
using namespace std;

#define print(v) {cout<<((v)->key)<<" "; for(auto&i:((v)->value))cout<<i<<" ";cout<<endl;}


template <class K = int, class V = CTree<K>>
void khop(AVL<K, V> &graph, vector<K> &all_transits, int n_samples) {
    Timer t;
    t.Start();
    int step_count = sample_size(-1) * n_samples;
    int prev_step_count = n_samples;
    int t_begin = 0;
    int old_t_begin = 0;
    for (int step = 0; step < steps(); step++) {
        t_begin += step_count;
        step_count *= sample_size(step);
        prev_step_count *= sample_size(step-1);
        for (int idx = 0; idx < step_count; idx++) {
            int t_idx = t_begin + idx;
            int old_t_idx = old_t_begin + idx / sample_size(step);
            // cout << "sample idx: " << idx / step_count << ", t_idx: " << t_idx << ", old_t_idx: " << old_t_idx << endl;
            K old_t = all_transits[old_t_idx];
            if (old_t == nptr()) {
            all_transits[t_idx] = nptr();
            continue;
            }
            all_transits[t_idx] = sample_next(graph, old_t);
        }
        old_t_begin += prev_step_count;
    }

    t.Stop();

    std::cout << "result size: " << step_count + t_begin << endl;
    std::cout << "Finished sampling in " << t.Seconds() << " sec" << endl;
}

int main() {
    // create example graph in fig 4 of aspen paper
    vector<vector<int>> adj_list = {{1,2}, {0,2}, {0,1,3,4,5}, {2,5}, {2,5}, {2,3,4}};
    set<int> heads = {1,2};
    int total_nodes = 6;

    AVL<int, CTree<int>> tree;
    for(int i = 0; i < total_nodes; i ++){
        CTree<int> ctree(heads, adj_list[i]);
        // print(ctree.avl->root);
        tree = *tree.insert(i, ctree);
    }

    // randomly sample initial transits
    int n_samples = num_samples();
    int n_threads = 1;

    vector<int> inits = get_initial_transits(sample_size(-1) * n_samples, total_nodes);

    // allocate space for all sampled transits sequentially by step
    int step_count = sample_size(-1) * n_samples;
    int total_count = step_count;
    for (int step = 0; step < steps(); step++) {
        step_count *= sample_size(step);
        total_count += step_count;
    }
    vector<int> transits(total_count, 0);
    for (int i = 0; i < inits.size(); i++) {
        transits[i] = inits[i];
    }

    // perform khop sampling
    khop(tree, transits, n_samples);

    cout << "Completed sampling!" << endl;
    std::cout << "results\n";
    int _size = sample_size(-1) * n_samples;
    int total_size = 0;
    for (int step = 0; step <= steps(); step++) {
        std::cout << "\n";
        for (int i = 0; i < _size; i++) {
            std::cout << transits[i + total_size] << " ";
            // cout << i + p_size << " ";
        }
        total_size += _size;
        _size *= sample_size(step);
    }
}