#include "timer.h"
#include "graph.h"
using namespace std;

int main(int argc, char* argv[]) {
    // // create example graph in fig 4 of aspen paper
    // vector<vector<int>> adj_list = {{1,2}, {0,2}, {0,1,3,4,5}, {2,5}, {2,5}, {2,3,4}};
    // set<int> heads = {1,2};
    // int total_nodes = 6;

    // AVL<int, CTree<int>> tree;
    // for(int i = 0; i < total_nodes; i ++){
    //     CTree<int> ctree(heads, adj_list[i]);
    //     // print(ctree.avl->root);
    //     tree = *tree.insert(i, ctree);
    // }

    Timer t;
    t.Start();

    // load graph given in argument into tree rep
    string in_prefix = argv[1];
    Graph g;
    load_graph(g, in_prefix);
    int n = g.n_vertices;
    int m = g.n_edges;

    ofstream fout((in_prefix + ".aspen.txt").c_str());
    fout << "AdjacencyGraph" << "\n";
    fout << n << "\n" << m << "\n";

    // vertex offsets
    for(int i = 0; i < n; i ++) {
      fout << g.vertices[i] << "\n";
    }
    //edges
    for(int i = 0; i < m; i ++) {
      fout << g.edges[i] << "\n";
    }

    fout.close();
    t.Stop();

    cout << "Finished translating in " << t.Seconds() << " sec" << endl;
}
