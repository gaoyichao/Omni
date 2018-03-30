#include <graph.h>
#include <assert.h>

/*
 * _graph_v_degree_aoi - 计算给定节点的出度、入度、所有度(All, Out, IN -> aoi)
 *
 * @g: 目标图
 * @vid: 考察节点索引
 * @dir: 边的考察方向
 * @loops: 是否记录自环边
 */
int _graph_v_degree_aoi(const struct Graph *g, int vid, EGraphEdgeDir dir, EGraphLoops loops) {
    int res = 0;

    if (EGEDGE_DIR_IN & dir)
        res += (VECTOR(g->is)[vid+1] - VECTOR(g->is)[vid]);
    if (EGEDGE_DIR_OUT & dir)
        res += (VECTOR(g->os)[vid+1] - VECTOR(g->os)[vid]);

    if (EGEDGE_LOOPS_YES == loops)
        return res;

    if (EGEDGE_DIR_IN & dir) {
        for (int j = VECTOR(g->is)[vid]; j<VECTOR(g->is)[vid+1]; j++) {
            if (VECTOR(g->from)[VECTOR(g->ii)[j]] == vid)
                res -= 1;
        }
    }
    if (EGEDGE_DIR_OUT & dir) {
        for (int j = VECTOR(g->os)[vid]; j < VECTOR(g->os)[vid+1]; j++) {
            if (VECTOR(g->to)[VECTOR(g->oi)[j]] == vid)
                res -= 1;
        }
    }

    return res;
}

/*
 * _graph_v_degree_bidir - 计算给定节点的双向度(双向边对数)
 *
 * @g: 目标图
 * @vid: 考察节点索引
 * @loops: 自环边是否看作是一种双向边
 */
int _graph_v_degree_bidir(const struct Graph *g, int vid, EGraphLoops loops) {
    int res = 0;

    vector_int ineis, oneis;
    vector_int_init(&ineis, 0);
    vector_int_init(&oneis, 0);
    graph_neighbors(g, &ineis, vid, EGEDGE_DIR_IN);
    graph_neighbors(g, &oneis, vid, EGEDGE_DIR_OUT);
    int n = vector_int_size(&oneis);

    // 既是入又是出的节点就是双向节点
    for (int i = 0; i < n; i++) {
        int v = VECTOR(oneis)[i];
        if (EGEDGE_LOOPS_NO == loops && v == vid)
            continue;
        if (vector_int_contains(&ineis, v))
            res++;
    }

    vector_int_destroy(&oneis);
    vector_int_destroy(&ineis);
    return res;
}
/*
 * _graph_v_degree_all_nobidir - 计算给定节点的所有度,双向边只记录一次
 *
 * @g: 目标图
 * @vid: 考察节点索引
 * @dir: 边的考察方向,只能是EGEDGE_DIR_BOTH_NOBIDIR
 * @loops: 是否记录自环边
 */
int _graph_v_degree_all_nobidir(const struct Graph *g, int vid, EGraphEdgeDir dir, EGraphLoops loops) {
    assert(EGEDGE_DIR_BOTH_NOBIDIR == dir);
    int bidir = _graph_v_degree_bidir(g, vid, loops);
    int aoi = _graph_v_degree_aoi(g, vid, dir, loops);
    return aoi - bidir;
}
/*
 * graph_v_degree - 计算给定节点的各种度
 *
 * @g: 目标图
 * @vid: 考察节点索引
 * @dir: 边的考察方向
 *       EGEDGE_DIR_OUT - 出度
 *       EGEDGE_DIR_IN - 入度
 *       EGEDGE_DIR_BOTH - 度
 *       EGEDGE_BIDIR - 双向边数量
 *       EGEDGE_DIR_BOTH_NOBIDIR - 度、双向边只记录一次
 * @loops: 是否记录自环边
 */
int graph_v_degree(const struct Graph *g, int vid, EGraphEdgeDir dir, EGraphLoops loops) {
    assert(0 <= vid);
    assert(vid < graph_vertices_count(g));
    switch (dir) {
    case EGEDGE_DIR_OUT:
    case EGEDGE_DIR_IN:
    case EGEDGE_DIR_BOTH:
        return _graph_v_degree_aoi(g, vid, dir, loops);
    case EGEDGE_DIR_BIDIR:
        return _graph_v_degree_bidir(g, vid, loops);
    case EGEDGE_DIR_BOTH_NOBIDIR:
        return _graph_v_degree_all_nobidir(g, vid, dir, loops);
        break;
    default:
        assert(0);
    }
    return 0;
}
