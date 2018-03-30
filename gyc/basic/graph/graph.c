#include <graph.h>
#include <mm.h>
#include <assert.h>

/*
 * create_graph - 创建一个图
 *
 * @edge: 边描述,起始节点与终止节点成对出现
 * @n: 节点数量
 * @isDir: 是否为有向图
 */
struct Graph *create_graph(const vector_int *edges, int n, int isDir) {
    assert(0 == vector_int_size(edges) % 2);
    assert(0 <= vector_int_min(edges));

    int max = vector_int_max(edges);
    n = (n > max) ? n : (max + 1);

    struct Graph *g = create_empty_graph(n, isDir);
    if (vector_int_size(edges) > 0)
        graph_add_edges(g, edges);

    return g;
}
/*
 * create_empty_graph - 创建一个空图
 *
 * @n: 节点数量
 * @isDir: 是否为有向图
 */
struct Graph * create_empty_graph(int n, int isDir) {
    assert(0 <= n);

    struct Graph *g = Calloc(1, struct Graph);
    g->n = 0;
    g->directed = isDir;
    vector_int_init(&(g->from),0);
    vector_int_init(&(g->to),  0);
    vector_int_init(&(g->oi),  0);
    vector_int_init(&(g->ii),  0);
    vector_int_init(&(g->os),  1);
    vector_int_init(&(g->is),  1);

    VECTOR(g->os)[0] = 0;
    VECTOR(g->is)[0] = 0;

    graph_add_vertices(g, n);

    vector_voidptr_init(&(g->wv), 0);
    vector_voidptr_init(&(g->we), 0);

    return g;
}
/*
 * destroy_graph - 回收网络内存
 *
 * @g: 目标图
 */
void destroy_graph(struct Graph *g) {
    vector_int_destroy(&(g->from));
    vector_int_destroy(&(g->to));
    vector_int_destroy(&(g->oi));
    vector_int_destroy(&(g->ii));
    vector_int_destroy(&(g->os));
    vector_int_destroy(&(g->is));

    int n = vector_voidptr_size(&(g->wv));
    for (int i = 0; i < n; i++)
        Free(VECTOR(g->wv)[i]);
    vector_voidptr_destroy(&(g->wv));
    n = vector_voidptr_size(&(g->we));
    for (int i = 0; i < n; i++)
        Free(VECTOR(g->we)[i]);
    vector_voidptr_destroy(&(g->we));
    Free(g);
}
/*
 * graph_reverse - 反转图
 *
 * @g: 目标图
 */
int graph_reverse(struct Graph*g) {
    vector_int tmp;

    tmp = g->from;
    g->from = g->to;
    g->to = tmp;

    tmp = g->oi;
    g->oi = g->ii;
    g->ii = tmp;

    tmp = g->os;
    g->os = g->is;
    g->is = tmp;

    return 0;
}

/*
 * graph_edges_count - 获取边数量
 *
 * @g: 目标图
 */
int graph_edges_count(const struct Graph *g) {
    return vector_int_size(&(g->from));
}

/*
 * graph_vertices_count - 获取节点数量
 */
int graph_vertices_count(const struct Graph *g)
{
    return g->n;
}

/*
 * graph_is_directed - 判定网络是否为有向图
 */
int graph_is_directed(const struct Graph *g)
{
    return g->directed;
}

/*
 * graph_edge - 获取图中指定eid的边
 *
 * @g: 目标图
 * @eid: 边索引
 * @from: 边起始节点
 * @to: 边终止节点
 */
int graph_edge(const struct Graph *g, int eid, int *from, int *to) {
    *from = GRAPH_EDGE_FROM(g, eid);
    *to = GRAPH_EDGE_TO(g, eid);

    if (!graph_is_directed(g) && *from > *to) {
        int tmp = *from;
        *from = *to;
        *to = tmp;
    }
    return 0;
}
/*
 * graph_edges - 获取图中指定eids的边
 *
 * @g: 目标图
 * @eids: 图中边索引们
 * @edges: 边描述,起始节点与终止节点成对出现
 */
int graph_edges(const struct Graph *g, struct GraphEdgeSelector *eids, vector_int *edges) {
    int ptr = 0;
    int n = GRAPH_EIT_SIZE(eids);
    int ec = graph_edges_count(g);

    vector_int_resize(edges, n * 2);
    for (GRAPH_EIT_RESET(eids); !GRAPH_EIT_END(eids); GRAPH_EIT_NEXT(eids)) {
        int e = GRAPH_EIT_GET(eids);
        assert(e < ec);
        VECTOR(*edges)[ptr++] = GRAPH_EDGE_FROM(g, e);
        VECTOR(*edges)[ptr++] = GRAPH_EDGE_TO(g, e);
    }

    return 0;
}


/*
 * graph_add_vertices - 添加节点
 *
 * @g: 目标图
 * @nv: 节点数量
 */
int graph_add_vertices(struct Graph *g, int nv) {
    assert(0 <= nv);

    vector_int_resize(&(g->os), g->n + nv + 1);
    vector_int_resize(&(g->is), g->n + nv + 1);

    int ec = graph_edges_count(g);
    for (int i = g->n + 1; i < g->n + nv + 1; i++) {
        VECTOR(g->os)[i] = ec;
        VECTOR(g->is)[i] = ec;
    }
    g->n += nv;

    return 0;
}

/*
 * graph_add_edges - 添加边
 *
 * @g: 目标图
 * @edge: 边描述,起始节点与终止节点成对出现
 */
int graph_add_edges(struct Graph *g, const vector_int *edges)
{
    assert(0 == vector_int_size(edges) % 2);
    assert(1 == vector_int_isininterval(edges, 0, graph_vertices_count(g)-1));

    int ec = graph_edges_count(g);
    int edge_size = vector_int_size(edges);
    int ec_to_add = edge_size / 2;


    /* from & to */
    vector_int_reserve(&(g->from), ec + ec_to_add);
    vector_int_reserve(&(g->to), ec + ec_to_add);
    int i = 0;
    int directed = g->directed;
    while (i < edge_size) {
        if (GRAPH_DIRECTED == directed || VECTOR(*edges)[i] > VECTOR(*edges)[i+1]) {
            vector_int_push_back(&(g->from), VECTOR(*edges)[i++]);
            vector_int_push_back(&(g->to),   VECTOR(*edges)[i++]);
        } else {
            vector_int_push_back(&(g->to),   VECTOR(*edges)[i++]);
            vector_int_push_back(&(g->from), VECTOR(*edges)[i++]);
        }
    }

    /* oi & ii */
    vector_int newoi, newii;
    vector_int_init(&newoi, ec + ec_to_add);
    vector_int_init(&newii, ec + ec_to_add);
    vector_int_order_inc2(&(g->from), &(g->to), &newoi, g->n);
    vector_int_order_inc2(&(g->to), &(g->from), &newii, g->n);


    /* os & is */
    vector_int_scan_tie(&(g->os), &(g->from), &newoi, g->n);
    vector_int_scan_tie(&(g->is), &(g->to),   &newii, g->n);

    /* everything went fine */
    vector_int_destroy(&(g->oi));
    vector_int_destroy(&(g->ii));
    g->oi = newoi;
    g->ii = newii;

    return 0;
}
/*
 * graph_attach_vertice_weight - 附加指定节点的权
 *
 * @g: 目标图
 * @vid: 目标节点索引
 * @pw: 指向权内容的指针
 * @size: 权内容字节数量
 */
int graph_attach_vertice_weight(struct Graph *g, int vid, const void *pw, int size)
{
    int vc = graph_vertices_count(g);
    assert(0 <= vid && vid < vc);

    if (vector_voidptr_size(&(g->wv)) == 0) {
        vector_voidptr_resize(&(g->wv), vc);
        vector_voidptr_null(&(g->wv));
    }

    if (0 != VECTOR(g->wv)[vid])
        free(VECTOR(g->wv)[vid]);

    VECTOR(g->wv)[vid] = malloc(size);
    memcpy(VECTOR(g->wv)[vid], pw, size);
    return 0;
}
/*
 * graph_attach_vertice_weight - 附加指定边的权
 *
 * @g: 目标图
 * @eid: 目标边索引
 * @pw: 指向权内容的指针
 * @size: 权内容字节数量
 */
int graph_attach_edge_weight(struct Graph *g, int eid, const void *pw, int size)
{
    int ec = graph_edges_count(g);
    assert(0 <= eid && eid < ec);

    if (vector_voidptr_size(&(g->we)) == 0) {
        vector_voidptr_resize(&(g->we), ec);
        vector_voidptr_null(&(g->we));
    }

    if (0 != VECTOR(g->we)[eid])
        free(VECTOR(g->we)[eid]);

    VECTOR(g->we)[eid] = malloc(size);
    memcpy(VECTOR(g->we)[eid], pw, size);
    return 0;
}
/*
 * graph_del_edges - 删除边
 *
 * @g: 目标图
 * @eids: 边选择器
 */
int graph_del_edges(struct Graph *g, struct GraphEdgeSelector *eids) {
    int ec = graph_edges_count(g);
    int vc = graph_vertices_count(g);
    // 标记需要移除的边
    int *mark = Calloc(ec, int);
    assert(0 != mark);
    int edges_to_remove = 0;

    // 记录需要移除的边
    for (GRAPH_EIT_RESET(eids); !GRAPH_EIT_END(eids); GRAPH_EIT_NEXT(eids)) {
        int e = GRAPH_EIT_GET(eids);
        if (0 == mark[e]) {
            edges_to_remove++;
            mark[e]++;
        }
    }
    int remaining_edges = ec - edges_to_remove;
    // 重新创建一个图
    vector_int newfrom, newto, newoi, newii;
    vector_int_init(&newfrom, remaining_edges);
    vector_int_init(&newto, remaining_edges);
    vector_int_init(&newoi, remaining_edges);
    vector_int_init(&newii, remaining_edges);
    for (int i = 0, j = 0; j < remaining_edges; i++) {
        if (0 == mark[i]) {
            VECTOR(newfrom)[j] = VECTOR(g->from)[i];
            VECTOR(newto)[j] = VECTOR(g->to)[i];
            j++;
        }
    }
    vector_int_order_inc2(&newfrom, &newto, &newoi, vc);
    vector_int_order_inc2(&newto, &newfrom, &newii, vc);
    /* os & is */
    vector_int_scan_tie(&(g->os), &newfrom, &newoi, g->n);
    vector_int_scan_tie(&(g->is), &newto,   &newii, g->n);
    // 释放原来的边描述
    vector_int_destroy(&g->from);
    vector_int_destroy(&g->to);
    vector_int_destroy(&g->oi);
    vector_int_destroy(&g->ii);

    g->from = newfrom;
    g->to = newto;
    g->oi = newoi;
    g->ii = newii;

    // 删除权
    if (0 != vector_voidptr_size(&(g->we))) {
        vector_voidptr newwe;
        vector_voidptr_init(&newwe, remaining_edges);
        for (int i = 0, j = 0; j < remaining_edges; i++) {
            if (0 == mark[i]) {
                VECTOR(newwe)[j] = VECTOR(g->we)[i];
                j++;
            } else {
                if (0 != VECTOR(g->we)[i])
                    Free(VECTOR(g->we)[i]);
            }
        }
        vector_voidptr_destroy(&g->we);
        g->we = newwe;
    }

    Free(mark);
    return 0;
}



void print_graph_vectors(const struct Graph *g, FILE *f)
{
    fprintf(f, "\n");
    print_vector_int(&(g->from),f);
    print_vector_int(&(g->to),f);
    print_vector_int(&(g->oi),f);
    print_vector_int(&(g->ii),f);
    print_vector_int(&(g->os),f);
    print_vector_int(&(g->is),f);
    fprintf(f, "\n");
}
