#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdint>
// using namespace std

struct Graph {
    int64_t n_edges;
    int vid_size, eid_size, vlabel_size, elabel_size;
    int feat_len, num_vertex_classes, num_edge_classes;
    uint32_t max_degree, n_vertices;
    uint32_t *edges;
    int64_t *vertices;
};

void read_meta_info(Graph &g, std::string prefix) {
    std::ifstream f_meta((prefix + ".meta.txt").c_str());
    assert(f_meta);
    int64_t nv = 0;
    f_meta >> nv;
    f_meta >> g.n_edges >> g.vid_size >> g.eid_size >> g.vlabel_size >> g.elabel_size >> g.max_degree >> g.feat_len >> g.num_vertex_classes >> g.num_edge_classes;
    assert(sizeof(uint32_t) == g.vid_size);
    assert(sizeof(int64_t) == g.eid_size);
    assert(sizeof(uint8_t) == g.vlabel_size);
    // assert(sizeof(elabel_t) == elabel_size);
    f_meta.close();
    assert(nv > 0 && g.n_edges > 0);
    if (g.vid_size == 4)
        assert(nv < 4294967295);
    g.n_vertices = nv;
    std::cout << "Reading graph: |V| " << nv << " |E| " << g.n_edges << "\n";
}

template<typename T>
T* custom_alloc_global(size_t elements) {
    return new T[elements];
}

template <typename T>
static void read_file(Graph &g, std::string fname, T *&pointer, int length) {
    pointer = custom_alloc_global<T>(length);
    assert(pointer);
    std::ifstream inf(fname.c_str(), std::ios::binary);
    if (!inf.good())
    {
        std::cerr << "Failed to open file: " << fname << "\n";
        exit(1);
    }
    inf.read(reinterpret_cast<char *>(pointer), sizeof(T) * length);
    inf.close();
}

void load_row_pointers(Graph &g, std::string prefix)
{
    read_file(g, prefix + ".vertex.bin", g.vertices, g.n_vertices + 1);
}

void load_graph_data(Graph &g, std::string prefix) {
    // read row pointers
    load_row_pointers(g, prefix);

    // read column indices
    read_file(g, prefix + ".edge.bin", g.edges, g.n_edges);
}

void load_graph(Graph &g, std::string prefix) {
    // read meta information
    read_meta_info(g, prefix);

    // load graph data
    load_graph_data(g, prefix);
}