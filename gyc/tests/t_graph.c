#include <t_graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void T_graph(void) {
    printf("\n******************************\n");
    printf("    T_graph 测试开始\n");
    printf("******************************\n");


    vector_int v1;
    vector_int_init_value_end(&v1, -1,
                              1,5,
                              2,5,
                              1,2,
                              1,2,
                              0,3,
                              0,2,
                              3,4,
                              3,6,
                              4,6,
                              6,4,
                              7,7,
                              7,7, -1);
    struct Graph *g1 = create_graph(&v1, 23, GRAPH_DIRECTED);
    assert(23 == graph_vertices_count(g1));
    assert(GRAPH_DIRECTED == graph_is_directed(g1));
    assert(12 == graph_edges_count(g1));
    assert(graph_edge_contains(g1, 1,5,EGEDGE_DIR_OUT));
    assert(graph_edge_contains(g1, 5,1,EGEDGE_DIR_IN));
    assert(graph_edge_contains(g1, 5,1,EGEDGE_DIR_BOTH));
    assert(!graph_edge_contains(g1, 2,1,EGEDGE_DIR_OUT));
    assert(!graph_edge_contains(g1, 7,1,EGEDGE_DIR_OUT));
    assert(graph_has_multiple_edge(g1));
    print_graph_vectors(g1, stdout);
    printf("nedges: %d\n", graph_edges_count(g1));
    vector_int v2;
    vector_int_init(&v2, 0);
    graph_neighbors(g1, &v2, 2, EGEDGE_DIR_IN);
    printf("IN neighbors of %d:\n", 2);
    print_vector_int(&v2,stdout);
    graph_neighbors(g1, &v2, 2, EGEDGE_DIR_OUT);
    printf("OUT neighbors of %d:\n", 2);
    print_vector_int(&v2,stdout);
    graph_neighbors(g1, &v2, 2, EGEDGE_DIR_BOTH);
    printf("BOTH neighbors of %d:\n", 2);
    print_vector_int(&v2,stdout);

    printf("节点1出发的边索引:");
    graph_edges_from(g1, 1, &v2);
    print_vector_int(&v2, stdout);

    printf("终止于节点2的边索引:");
    graph_edges_to(g1, 2, &v2);
    print_vector_int(&v2, stdout);

    vector_int_destroy(&v2);
    vector_int_init_value_end(&v2, -1, 0, 1, 4, -1);
    struct GraphEdgeSelector *eids = create_graph_edge_selector(EGEDGE_IT_TYPE_VECTOR);
    graph_edge_selector_append(eids, &v2);
    graph_del_edges(g1, eids);
    assert(9 == graph_edges_count(g1));
    assert(!graph_edge_contains(g1, 1,5,EGEDGE_DIR_OUT));
    assert(!graph_edge_contains(g1, 5,2,EGEDGE_DIR_IN));
    printf("删除指定边:");
    print_graph_vectors(g1, stdout);
    free(eids);

    struct Graph *g2 = create_graph(&v1, 23, GRAPH_DIRECTED);
    assert(23 == graph_vertices_count(g2));
    assert(GRAPH_DIRECTED == graph_is_directed(g2));
    assert(12 == graph_edges_count(g2));
    assert(graph_edge_contains(g2, 1,5,EGEDGE_DIR_OUT));
    assert(graph_edge_contains(g2, 5,1,EGEDGE_DIR_IN));
    assert(graph_edge_contains(g2, 5,1,EGEDGE_DIR_BOTH));
    assert(!graph_edge_contains(g2, 2,1,EGEDGE_DIR_OUT));
    assert(!graph_edge_contains(g2, 7,1,EGEDGE_DIR_OUT));
    assert(graph_has_multiple_edge(g2));
    vector_int_destroy(&v2);
    vector_int_init(&v2, 0);
    graph_neighbors(g2, &v2, 7, EGEDGE_DIR_BOTH_NOBIDIR);
    printf("V7的邻接节点数量(无双向边):%d\n", vector_int_size(&v2));
    vector_int_destroy(&v2);
    vector_int_init(&v2, 0);
    graph_neighbors(g2, &v2, 7, EGEDGE_DIR_IN);
    printf("V7的入节点数量:%d\n", vector_int_size(&v2));
    graph_remove_multi_edges(g2);
    assert(!graph_has_multiple_edge(g2));
    printf("删除重复边:");
    print_graph_vectors(g2, stdout);
    vector_int_destroy(&v2);
    vector_int_init(&v2, 0);
    graph_neighbors(g2, &v2, 7, EGEDGE_DIR_BOTH_NOBIDIR);
    printf("V7的邻接节点数量(无双向边):%d\n", vector_int_size(&v2));
    vector_int_destroy(&v2);
    vector_int_init(&v2, 0);
    graph_neighbors(g2, &v2, 7, EGEDGE_DIR_IN);
    printf("V7的入节点数量:%d\n\n", vector_int_size(&v2));

    struct Graph *g3 = create_graph(&v1, 23, GRAPH_DIRECTED);
    assert(23 == graph_vertices_count(g3));
    assert(GRAPH_DIRECTED == graph_is_directed(g3));
    assert(12 == graph_edges_count(g3));
    assert(graph_edge_contains(g3, 1,5,EGEDGE_DIR_OUT));
    assert(graph_edge_contains(g3, 5,1,EGEDGE_DIR_IN));
    assert(graph_edge_contains(g3, 5,1,EGEDGE_DIR_BOTH));
    assert(!graph_edge_contains(g3, 2,1,EGEDGE_DIR_OUT));
    assert(!graph_edge_contains(g3, 7,1,EGEDGE_DIR_OUT));
    assert(graph_has_multiple_edge(g3));
    printf("V7的双向边数量:%d\n", graph_v_degree(g3, 7, EGEDGE_DIR_BIDIR, EGEDGE_LOOPS_YES));
    printf("V7的双向边数量(无自环边):%d\n", graph_v_degree(g3, 7, EGEDGE_DIR_BIDIR, EGEDGE_LOOPS_NO));
    printf("V7的出度(自环边):%d\n",graph_v_degree(g3, 7, EGEDGE_DIR_OUT, EGEDGE_LOOPS_YES));
    printf("V7的出度(无自环边):%d\n",graph_v_degree(g3, 7, EGEDGE_DIR_OUT, EGEDGE_LOOPS_NO));
    printf("V7的入度(自环边):%d\n",graph_v_degree(g3, 7, EGEDGE_DIR_IN, EGEDGE_LOOPS_YES));
    printf("V7的入度(无自环边):%d\n",graph_v_degree(g3, 7, EGEDGE_DIR_IN, EGEDGE_LOOPS_NO));
    printf("V7的度(自环边):%d\n",graph_v_degree(g3, 7, EGEDGE_DIR_BOTH, EGEDGE_LOOPS_YES));
    printf("V7的度(无自环边):%d\n",graph_v_degree(g3, 7, EGEDGE_DIR_BOTH, EGEDGE_LOOPS_NO));
    printf("V7的度(无双向边,自环):%d\n",graph_v_degree(g3, 7, EGEDGE_DIR_BOTH_NOBIDIR, EGEDGE_LOOPS_YES));
    printf("V7的度(无双向边,无自环):%d\n",graph_v_degree(g3, 7, EGEDGE_DIR_BOTH_NOBIDIR, EGEDGE_LOOPS_NO));

    print_vector_int(&v2, stdout);
    printf("V4的双向边数量:%d\n", graph_v_degree(g3, 4, EGEDGE_DIR_BIDIR, EGEDGE_LOOPS_YES));
    printf("V4的双向边数量(无自环边):%d\n", graph_v_degree(g3, 4, EGEDGE_DIR_BIDIR, EGEDGE_LOOPS_NO));
    printf("V4的出度(自环边):%d\n",graph_v_degree(g3, 4, EGEDGE_DIR_OUT, EGEDGE_LOOPS_YES));
    printf("V4的出度(无自环边):%d\n",graph_v_degree(g3, 4, EGEDGE_DIR_OUT, EGEDGE_LOOPS_NO));
    printf("V4的入度(自环边):%d\n",graph_v_degree(g3, 4, EGEDGE_DIR_IN, EGEDGE_LOOPS_YES));
    printf("V4的入度(无自环边):%d\n",graph_v_degree(g3, 4, EGEDGE_DIR_IN, EGEDGE_LOOPS_NO));
    printf("V4的度(自环边):%d\n",graph_v_degree(g3, 4, EGEDGE_DIR_BOTH, EGEDGE_LOOPS_YES));
    printf("V4的度(无自环边):%d\n",graph_v_degree(g3, 4, EGEDGE_DIR_BOTH, EGEDGE_LOOPS_NO));
    printf("V4的度(无双向边,自环):%d\n",graph_v_degree(g3, 4, EGEDGE_DIR_BOTH_NOBIDIR, EGEDGE_LOOPS_YES));
    printf("V4的度(无双向边,无自环):%d\n",graph_v_degree(g3, 4, EGEDGE_DIR_BOTH_NOBIDIR, EGEDGE_LOOPS_NO));

    graph_reverse(g3);
    assert(graph_edge_contains(g3, 1,2,EGEDGE_DIR_IN));
    assert(graph_edge_contains(g3, 0,2,EGEDGE_DIR_IN));


    struct Graph *g4 = create_graph(&v1, 23, GRAPH_UNDIRECTED);
    assert(23 == graph_vertices_count(g4));
    assert(GRAPH_UNDIRECTED == graph_is_directed(g4));
    assert(12 == graph_edges_count(g4));
    assert(graph_edge_contains(g4, 1,5,EGEDGE_DIR_OUT));
    assert(graph_edge_contains(g4, 1,5,EGEDGE_DIR_IN));
    assert(graph_edge_contains(g4, 1,5,EGEDGE_DIR_BOTH));
    assert(graph_edge_contains(g4, 5,1,EGEDGE_DIR_OUT));
    assert(graph_edge_contains(g4, 5,1,EGEDGE_DIR_BOTH));
    assert(!graph_edge_contains(g4, 7,1,EGEDGE_DIR_OUT));
    assert(graph_has_multiple_edge(g4));

    destroy_graph(g1);
    destroy_graph(g2);
    destroy_graph(g3);
    destroy_graph(g4);
    printf("  T_graph 测试结束!\n");
    printf("******************************\n");
}

void T_graph_weight(void) {
    printf("\n******************************\n");
    printf("    T_graph_weight 测试开始\n");
    printf("******************************\n");

    vector_int v1;
    vector_int_init_value_end(&v1, -1,
                              1,5,
                              2,5,
                              1,2,
                              1,2,
                              0,3,
                              0,2,
                              3,4,
                              3,6,
                              4,6,
                              6,4,
                              7,7,
                              7,7, -1);
    struct Graph *g1 = create_graph(&v1, 23, GRAPH_DIRECTED);
    float w1 = 12.3f;
    graph_attach_vertice_weight(g1, 4, &w1, sizeof(float));
    assert(0 == GraphVerWeightPtr(g1, 2, float));
    assert(w1 == GraphVerWeight(g1, 4, float));


    printf("  T_graph_weight 测试结束!\n");
    printf("******************************\n");
}
