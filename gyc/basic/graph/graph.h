#ifndef GRAPH_H
#define GRAPH_H

#include <vector_int.h>
#include <vector_voidptr.h>
#include <graph_iterator.h>

/*
 * Graph - 以edge list的形式描述图
 *
 * 1. 网络中的第i条边的访问形式：from[i] -- to[i]
 * 2. 从节点v出发的第一条边：from[oi[os[v]]] -- to[ii[os[v]]]
 * 3. 指向节点v的第一条边：from[oi[is[v]]] -- to[ii[is[v]]]
 * 4. 对于无向图，每条无向边只存储一次。查询节点v1和v2之间是否有边，
 *    需要正向反向两次查询。
 */
struct Graph
{
    int n;              // 节点数量,节点总是从0开始索引的
    int directed;       // 是否为有向图
    vector_int from;    // 边的起始节点编号
    vector_int to;      // 边的终止节点编号
    vector_int oi;      // 对起始节点的排序
    vector_int ii;      // 对终止节点的排序
    vector_int os;      // 起始节点的边索引,长度为n+1
    vector_int is;      // 终止节点的边索引,长度为n+1

    vector_voidptr wv;  // 节点权重
    vector_voidptr we;  // 边权重
};

#define GraphVerWeightPtr(g, vid, type) ((type *)(VECTOR((g)->wv)[vid]))
#define GraphVerWeight(g, vid, type) (*GraphVerWeightPtr(g, vid, type))

typedef enum EGraphDirected
{
    GRAPH_UNDIRECTED=0,
    GRAPH_DIRECTED=1
} EGraphDirected;

typedef enum EGraphLoops
{
    EGEDGE_LOOPS_NO=0,
    EGEDGE_LOOPS_YES=1
} EGraphLoops;

typedef enum EGraphEdgeDir
{
    EGEDGE_DIR_OUT=1,
    EGEDGE_DIR_IN=2,
    EGEDGE_DIR_BOTH=3,
    EGEDGE_DIR_BIDIR=4,
    EGEDGE_DIR_BOTH_NOBIDIR=11
} EGraphEdgeDir;

struct Graph *create_graph(const vector_int *edges, int n, int isDir);
struct Graph * create_empty_graph(int n, int isDir);
void destroy_graph(struct Graph *g);

int graph_reverse(struct Graph*g);

int graph_edges_count(const struct Graph *g);
int graph_vertices_count(const struct Graph *g);
int graph_is_directed(const struct Graph *g);

#define GRAPH_EDGE_FROM(g, e)   ((VECTOR((g)->from)[(e)]))
#define GRAPH_EDGE_TO(g, e) ((VECTOR((g)->to)[(e)]))
int graph_edge(const struct Graph *g, int eid, int *from, int *to);
int graph_edges(const struct Graph *g, struct GraphEdgeSelector *eids, vector_int *edges);
int graph_edges_from(const struct Graph *g, int vid, vector_int *eids);
int graph_edges_to(const struct Graph *g, int vid, vector_int *eids);

int graph_add_vertices(struct Graph *g, int nv);
int graph_add_edges(struct Graph *g, const vector_int *edges);
int graph_attach_vertice_weight(struct Graph *g, int vid, const void *pw, int size);
int graph_attach_edge_weight(struct Graph *g, int eid, const void *pw, int size);
int graph_del_edges(struct Graph *g, struct GraphEdgeSelector *eids);

int graph_edge_contains(const struct Graph *g, int v, int w, EGraphEdgeDir dir);
int graph_neighbors(const struct Graph *g, vector_int *neis, int v, EGraphEdgeDir dir);
int graph_has_multiple_edge(const struct Graph *g);
int graph_remove_multi_edges(struct Graph *g);

int _graph_v_degree_aoi(const struct Graph *g, int vid, EGraphEdgeDir dir, EGraphLoops loops);
int _graph_v_degree_bidir(const struct Graph *g, int vid, EGraphLoops loops);
int _graph_v_degree_all_nobidir(const struct Graph *g, int vid, EGraphEdgeDir dir, EGraphLoops loops);
int graph_v_degree(const struct Graph *g, int vid, EGraphEdgeDir dir, EGraphLoops loops);

void print_graph_vectors(const struct Graph *g, FILE *f);


#endif // GRAPH_H
