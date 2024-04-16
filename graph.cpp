#include "graph.h"
using namespace std;

int main(int argc, char* argv[]) {
    Graph g;
    string in_prefix = argv[1];
    load_graph(g, in_prefix);
    cout << "num_vertices: " << g.n_vertices << ", num_edges: " << g.n_edges << endl;
    // g.vertices stores csr rowptrs for edge list
    // for (int i = 0; i < g.n_vertices + 1; i++) {
    //     cout << " " << g.vertices[i];
    // }
    // cout << endl;
    // g.edges stores csr edge list
    // for (int i = 0; i < g.n_edges; i++) {
    //     cout << " " << g.edges[i];
    // }
    // cout << endl;
    // cout << g.get_num_edges(2) << endl;
}