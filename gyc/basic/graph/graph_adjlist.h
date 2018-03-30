#ifndef GRAPH_ADJLIST_H
#define GRAPH_ADJLIST_H

#include <vector_int.h>
#include <graph.h>

typedef vector_int* pVectorInt;

/*
 * GraphAdjList - 以邻接表的形式描述图
 */
struct GraphAdjList
{
    int len;            // 邻接表长
    EGraphEdgeDir dir;  // 邻接表中边的方向
    pVectorInt adjs;    // 每个邻接表对应一个节点
};

#define graph_adjlist_get(al,no) (&(al)->adjs[(no)])

struct GraphAdjList *create_graph_adjlist(const struct Graph *g, EGraphEdgeDir dir);
void graph_adjlist_destroy(struct GraphAdjList *al);
struct GraphAdjList *create_graph_adjlist_empty(int nv, EGraphEdgeDir dir);


#endif // GRAPH_ADJLIST_H
