#include <graph.h>
#include <mm.h>
#include <assert.h>

/*
 * _graph_neighbors - 计算给定节点的邻居,重复的节点重复记录
 *
 * @g: 目标图
 * @neis: 邻居节点
 * @v: 给定节点
 * @dir: 求取邻居的方式,GRAPH_DIR_IN只计算入度节点,
 *        GRAPH_DIR_OUT只计算出度节点,GRAPH_DIR_BOTH两个
 */
static int _graph_neighbors(const struct Graph *g, vector_int *neis, int v, EGraphEdgeDir dir)
{
    assert(EGEDGE_DIR_IN == dir || EGEDGE_DIR_OUT == dir || EGEDGE_DIR_BOTH == dir);

    int length = 0;
    if (dir & EGEDGE_DIR_OUT)
        length += (VECTOR(g->os)[v+1] - VECTOR(g->os)[v]);
    if (dir & EGEDGE_DIR_IN)
        length += (VECTOR(g->is)[v+1] - VECTOR(g->is)[v]);
    vector_int_resize(neis, length);

    int idx = 0;
    if (!graph_is_directed(g) || dir != EGEDGE_DIR_BOTH) {
        if (dir & EGEDGE_DIR_OUT) {
            int j = VECTOR(g->os)[v+1];
            for (int i = VECTOR(g->os)[v]; i < j; i++)
                VECTOR(*neis)[idx++] = VECTOR(g->to)[VECTOR(g->oi)[i]];
        }
        if (dir & EGEDGE_DIR_IN) {
            int j = VECTOR(g->is)[v+1];
            for (int i = VECTOR(g->is)[v]; i < j; i++)
                VECTOR(*neis)[idx++] = VECTOR(g->from)[VECTOR(g->ii)[i]];
        }
    } else {
        int j1 = VECTOR(g->os)[v+1];
        int i1 = VECTOR(g->os)[v];
        int j2 = VECTOR(g->is)[v+1];
        int i2 = VECTOR(g->is)[v];

        while (i1 < j1 && i2 < j2) {
            int n1 = VECTOR(g->to)[VECTOR(g->oi)[i1]];
            int n2 = VECTOR(g->from)[VECTOR(g->ii)[i2]];
            if (n1 < n2) {
                VECTOR(*neis)[idx++] = n1;
                i1++;
            } else if (n2 < n1) {
                VECTOR(*neis)[idx++] = n2;
                i2++;
            } else {
                VECTOR(*neis)[idx++] = n1;
                VECTOR(*neis)[idx++] = n2;
                i1++; i2++;
            }
        }

        while (i1 < j1) {
            int n1 = VECTOR(g->to)[VECTOR(g->oi)[i1]];
            VECTOR(*neis)[idx++] = n1;
            i1++;
        }
        while (i2 < j2) {
            int n2 = VECTOR(g->from)[VECTOR(g->ii)[i2]];
            VECTOR(*neis)[idx++] = n2;
            i2++;
        }
    }
    return 0;
}
/*
 * graph_neighbors_norepeat - 计算给定节点的邻居,不记录重复的节点
 *
 * @g: 目标图
 * @neis: 邻居节点
 * @v: 给定节点
 */
static int graph_neighbors_norepeat(const struct Graph *g, vector_int *neis, int v) {
    assert(v >= 0);
    assert(v < graph_vertices_count(g));

    int len = graph_v_degree(g, v, EGEDGE_DIR_BOTH_NOBIDIR, EGEDGE_LOOPS_NO);
    vector_int_resize(neis, len);

    int idx = 0;
    int tmp = -1;
    int j1 = VECTOR(g->os)[v+1];
    int i1 = VECTOR(g->os)[v];
    int j2 = VECTOR(g->is)[v+1];
    int i2 = VECTOR(g->is)[v];
    while (i1 < j1 && i2 < j2) {
        int n1 = VECTOR(g->to)[VECTOR(g->oi)[i1]];
        int n2 = VECTOR(g->from)[VECTOR(g->ii)[i2]];
        if (n1 < n2) {
            if (n1 != v && n1 != tmp) { tmp = n1; VECTOR(*neis)[idx++] = n1; }
            i1++;
        } else if (n2 < n1) {
            if (n2 != v && n2 != tmp) { tmp = n2; VECTOR(*neis)[idx++] = n2; }
            i2++;
        } else {
            if (n1 != v && n1 != tmp) { tmp = n1; 	VECTOR(*neis)[idx++] = n1; }
            i1++; i2++;
        }
    }
    while (i1 < j1) {
        int n1 = VECTOR(g->to)[VECTOR(g->oi)[i1]];
        if (n1 != v && n1 != tmp) { tmp = n1; VECTOR(*neis)[idx++] = n1; }
        i1++;
    }
    while (i2 < j2) {
        int n2 = VECTOR(g->from)[VECTOR(g->ii)[i2]];
        if (n2 != v && n2 != tmp) { tmp = n2; VECTOR(*neis)[idx++] = n2; }
        i2++;
    }

    return 0;
}

/*
 * graph_neighbors - 计算给定节点的邻居
 *
 * @g: 目标图
 * @neis: 邻居节点
 * @v: 给定节点
 * @dir: 求取邻居的方式,
 *        EGEDGE_DIR_IN只计算入度节点,
 *        EGEDGE_DIR_OUT只计算出度节点,
 *        EGEDGE_DIR_BOTH两个,
 *        EGEDGE_DIR_BOTH_NOBIDIR不包含重复节点
 */
int graph_neighbors(const struct Graph *g, vector_int *neis, int v, EGraphEdgeDir dir)
{
    assert(v >= 0);
    assert(v < graph_vertices_count(g));

    if (!g->directed)
        dir = EGEDGE_DIR_BOTH;

    if (EGEDGE_DIR_IN == dir || EGEDGE_DIR_OUT == dir || EGEDGE_DIR_BOTH == dir)
        return _graph_neighbors(g, neis, v, dir);
    else if (EGEDGE_DIR_BOTH_NOBIDIR == dir)
        return graph_neighbors_norepeat(g, neis, v);
    else
        assert(0);

    return 0;
}

/*
 * graph_edge_contains - 检查图中节点v与w之间是否有指定的关系
 *
 * @g: 目标图
 * @v: 节点
 * @w: 节点
 * @dir: 边链接关系
 *        EGEDGE_DIR_IN只计算入度节点,
 *        EGEDGE_DIR_OUT只计算出度节点,
 *        EGEDGE_DIR_BOTH两个
 * return: 1包含,0不包含
 */
int graph_edge_contains(const struct Graph *g, int v, int w, EGraphEdgeDir dir)
{
    assert(0 != g);
    int vc = graph_vertices_count(g);
    if (v >= vc || w >= vc)
        return 0;

    vector_int neis;
    vector_int_init(&neis, 0);
    graph_neighbors(g, &neis, v, dir);
    int res = vector_int_contains(&neis, w) ? 1 : 0;
    vector_int_destroy(&neis);

    return res;
}
/*
 * graph_edges_from - 获取图中从@vid出发的边索引
 *
 * @g: 目标图
 * @vid: 考察节点
 * @eids: 边索引
 */
int graph_edges_from(const struct Graph *g, int vid, vector_int *eids) {
    assert(vid >= 0);
    assert(vid < graph_vertices_count(g));

    int len = (VECTOR(g->os)[vid+1] - VECTOR(g->os)[vid]);
    vector_int_resize(eids, len);

    int idx = 0;
    int end = VECTOR(g->os)[vid+1];
    for (int i = VECTOR(g->os)[vid]; i < end; i++)
        VECTOR(*eids)[idx++] = VECTOR(g->oi)[i];
    return 0;
}
/*
 * graph_edges_from - 获取图中终止于@vid的边索引
 *
 * @g: 目标图
 * @vid: 考察节点
 * @eids: 边索引
 */
int graph_edges_to(const struct Graph *g, int vid, vector_int *eids) {
    assert(vid >= 0);
    assert(vid < graph_vertices_count(g));

    int len = (VECTOR(g->is)[vid+1] - VECTOR(g->is)[vid]);
    vector_int_resize(eids, len);

    int idx = 0;
    int end = VECTOR(g->is)[vid+1];
    for (int i = VECTOR(g->is)[vid]; i < end; i++)
        VECTOR(*eids)[idx++] = VECTOR(g->ii)[i];
    return 0;
}

/*
 * graph_has_multiple_edge - 判定网络中是否存在重复的边
 *
 * @g: 目标图
 */
int graph_has_multiple_edge(const struct Graph *g)
{
    int vc = graph_vertices_count(g);
    int ec = graph_edges_count(g);

    if (vc == 0 || ec == 0)
        return 0;

    vector_int neis;
    vector_int_init(&neis, 0);
    int found = 0, n;
    for (int i = 0; i < vc && !found; i++) {
        graph_neighbors(g, &neis, i, EGEDGE_DIR_OUT);
        n = vector_int_size(&neis);
        for (int j = 1; j < n; j++) {
            if (VECTOR(neis)[j-1] == VECTOR(neis)[j]) {
                if (graph_is_directed(g))
                    // 有向图,一定是重复边
                    return 1;
                else if (VECTOR(neis)[j-1] != i)
                    // 无向图,一条重复边,但不是自环
                    return 1;
                else if (j < n-1 && VECTOR(neis)[j] == VECTOR(neis)[j+1])
                    // 无向图中一条重复的自环
                    return 1;
            }
        }
    }
    return 0;
}

/*
 * graph_remove_multi_edges - 从网络中去除重复的边
 *
 * @g: 目标图
 */
int graph_remove_multi_edges(struct Graph *g) {
    int ec = graph_edges_count(g);
    int vc = graph_vertices_count(g);
    int *mark = Calloc(ec, int);
    assert(0 != mark);

    struct GraphEdgeSelector *es = create_graph_edge_selector(EGEDGE_IT_TYPE_VECTOR);
    // 遍历每个节点的边
    for (int i = 0; i < vc; i++) {
        graph_edges_from(g, i, &(es->vec));
        int n = vector_int_size(&(es->vec));
        for (int j = 1; j < n; j++) {
            int to_pre = GRAPH_EDGE_TO(g, VECTOR(es->vec)[j-1]);
            int to_cur = GRAPH_EDGE_TO(g, VECTOR(es->vec)[j]);
            int e = VECTOR(es->vec)[j];
            if (to_pre == to_cur)
                mark[e] = 1;
        }
    }
    vector_int_clear(&(es->vec));
    for (int i = 0; i < ec; i++) {
        if (mark[i])
            graph_edge_selector_push_back(es, i);
    }

    graph_del_edges(g, es);
    destroy_graph_edge_selector(es);
    Free(mark);

    return 0;
}



