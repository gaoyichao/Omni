#include <graph_adjlist.h>
#include <mm.h>
#include <assert.h>

/*
 * create_graph_adjlist - 根据给定的图创建一个邻接表
 *
 * @g: 目标图
 * @dir: 边的考察方向
 *       EGEDGE_DIR_OUT - 出度
 *       EGEDGE_DIR_IN - 入度
 *       EGEDGE_DIR_BOTH - 度
 *       EGEDGE_DIR_BOTH_NOBIDIR - 度、双向边只记录一次
 */
struct GraphAdjList *create_graph_adjlist(const struct Graph *g, EGraphEdgeDir dir) {
    assert(EGEDGE_DIR_OUT == dir || EGEDGE_DIR_IN == dir || EGEDGE_DIR_BOTH == dir || EGEDGE_DIR_BOTH_NOBIDIR == dir);

    struct GraphAdjList *al = Calloc(1, struct GraphAdjList);

    al->dir = dir;
    al->len = graph_vertices_count(g);
    al->adjs = Calloc(al->len, vector_int);
    assert(0 != al->adjs);

    vector_int tmp;
    vector_int_init(&tmp, 0);
    for (int i = 0; i < al->len; i++) {
        graph_neighbors(g, &tmp, i, dir);
        int n = vector_int_size(&tmp);
        vector_int_init(&(al->adjs[i]), n);
        for (int j = 0; j < n; j++)
            VECTOR(al->adjs[i])[j] = VECTOR(tmp)[j];
    }
    vector_int_destroy(&tmp);

    return al;
}
/*
 * graph_adjlist_destroy - 回收邻接表内存
 */
void graph_adjlist_destroy(struct GraphAdjList *al) {
    for (int i = 0; i < al->len; i++)
        if (0 != &al->adjs[i])
            vector_int_destroy(&(al->adjs[i]));
    Free(al->adjs);
    Free(al);
}
/*
 * create_graph_adjlist_empty - 创建一个空白的邻接表
 */
struct GraphAdjList *create_graph_adjlist_empty(int nv, EGraphEdgeDir dir) {
    assert(EGEDGE_DIR_OUT == dir || EGEDGE_DIR_IN == dir || EGEDGE_DIR_BOTH == dir || EGEDGE_DIR_BOTH_NOBIDIR == dir);

    struct GraphAdjList *al = Calloc(1, struct GraphAdjList);

    al->dir = dir;
    al->len = nv;
    al->adjs = Calloc(al->len, vector_int);
    assert(0 != al->adjs);

    for (int i = 0; i < al->len; i++)
        vector_int_init(&(al->adjs[i]),0);

    return al;
}
