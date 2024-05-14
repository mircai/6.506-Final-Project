#include "khop.h"
#include "timer.h"
#include "graph.h"
using namespace std;

#define print(v) {cout<<((v)->key)<<" "; for(auto&i:((v)->value))cout<<i<<" ";cout<<endl;}

inline double seconds() {
  struct timeval tp;
  struct timezone tzp;
  gettimeofday(&tp, &tzp);
  return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}

AVL<int, CTree<int>> load_csr_to_tree(Graph &g) {
    // make into tree
    Timer t;
    t.Start();
    AVL<int, CTree<int>> tree;
    cout << "test weofijowfjialwdijf" << endl;
    // int tot_edges = 0;
    for(int i = 0; i < g.n_vertices; i ++){
        int n_edges = g.get_num_edges(i);
        int b = max(100, (int)sqrt(n_edges)); // dynamic b
        int *edge_list = g.get_edges(i);
        CTree<int> ctree(edge_list, n_edges, b);
        // mutable initialization
        tree.root = tree._insert(tree.root, i, ctree, true);
    }
    t.Stop();

    cout << "Finished building in " << t.Seconds() << " sec" << endl;
    return tree;
}

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

    cout << "result size: " << step_count + t_begin << endl;
    cout << "Finished sampling in " << t.Seconds() << " sec" << endl;
}

int main(int argc, char* argv[]) {
    // load graph given in argument into tree rep
    string in_prefix = argv[1];
    Graph g;
    cout << "loading graph" << endl;
    load_graph(g, in_prefix);
    cout << "loaded graph" << endl;

    int total_nodes = g.n_vertices;
    AVL<int, CTree<int>> tree;

    // direct the graph
    // vector<vector<int>> edges(total_nodes);
    // for (int i = 0; i < total_nodes; i ++) {
    //   int n_edges = g.get_num_edges(i);
    //   int *edge_list = g.get_edges(i);
    //   for (int j = 0; j < n_edges; j ++) {
    //     if (i > j) edges[i].push_back(j);
    //   }
    // }

    // cout << "done directing graph" << endl;

    int b = 100; // EXPERIMENT WITH THIS
    // batch size = 10% of graph
    int bs = total_nodes / 10;

    double starttime = seconds();

    for (int bi = 0; bi < total_nodes; bi += bs) {
      // cout << "inserting batch " << (bi / bs) << endl;
      int end = min(bi + bs, total_nodes);
      int num_edges = 0;
      int num_old_edges = 0;
      for (int i = bi; i < end; i ++) {
        int n_edges = g.get_num_edges(i);
        int *edge_list = g.get_edges(i);
        vector<int> edges;
        for (int j = 0; j < n_edges; j ++) {
          int x = edge_list[j];
          if (x < end) {
            edges.push_back(x);
          }
          if (x < bi) {
            tree.get_node(x)->value._insert_mutable(i);
            num_old_edges ++;
          }
        }
        num_edges += edges.size();
        // for (auto&j : edges[i]) {
        //   auto p = tree.get_node(j);
        //   p->value._insert_mutable(i);
        // }
        // CTree<int> ctree(edge_list, n_edges, b);
        CTree<int> ctree(edges, b);
        tree.root = tree._insert(tree.root, i, ctree, true);
      }
      cout << (seconds() - starttime) << "\t" << num_edges << "\t" << num_old_edges << endl;
    }

    cout << "Finished building in " << (seconds()-starttime) << " sec" << endl;

    // randomly sample initial transits
    int n_samples = 40; //num_samples();
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

    // cout << "Completed sampling!" << endl;
    // cout << "results\n";
    // int _size = sample_size(-1) * n_samples;
    // int total_size = 0;
    // for (int step = 0; step <= steps(); step++) {
    //     cout << "\n";
    //     for (int i = 0; i < _size; i++) {
    //         cout << transits[i + total_size] << " ";
    //         // cout << i + p_size << " ";
    //     }
    //     total_size += _size;
    //     _size *= sample_size(step);
    // }
    // cout << endl;
}
