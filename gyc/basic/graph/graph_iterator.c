#include <graph.h>
#include <graph_iterator.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * create_graph_edge_selector - 构建一个边索引器
 */
struct GraphEdgeSelector* create_graph_edge_selector(EGraphEdgeITType ittype) {
    struct GraphEdgeSelector *es = (struct GraphEdgeSelector*)malloc(sizeof(struct GraphEdgeSelector));
    es->start = 0;
    es->end = 0;
    es->pos = 0;
    es->type = ittype;
    vector_int_init(&(es->vec), 0);
    return es;
}
/*
 * destroy_graph_edge_selector - 销毁一个边索引器
 *
 * @es: 目标边索引器
 */
void destroy_graph_edge_selector(struct GraphEdgeSelector *es) {
    vector_int_destroy(&(es->vec));
    free(es);
}

/*
 * graph_edge_selector_reset_vector - 初始化Vector形式的边索引器
 *
 * @es: 边索引器
 * @v: 索引向量
 */
void graph_edge_selector_reset_vector(struct GraphEdgeSelector *es, const vector_int *v) {
    es->start = 0;
    es->end = vector_int_size(v);
    es->pos = 0;
    es->type = EGEDGE_IT_TYPE_VECTOR;
    vector_int_destroy(&(es->vec));
    vector_int_init(&(es->vec), 0);
    vector_int_append(v, &(es->vec));
}
/*
 * graph_edge_selector_set_sequence - 初始化Sequence形式的边索引器
 *
 * @es: 边索引器
 * @start: 索引起点,包含
 * @end: 索引结尾,不包含
 */
void graph_edge_selector_reset_sequence(struct GraphEdgeSelector *es, int start, int end) {
    es->start = start;
    es->end = end;
    es->pos = start;
    es->type = EGEDGE_IT_TYPE_SEQ;
    vector_int_destroy(&(es->vec));
    vector_int_init(&(es->vec), 0);
}

/*
 * graph_edge_selector_push_back - 添加边索引
 *
 * @es: 目标索引器
 * @eid: 边索引
 */
void graph_edge_selector_push_back(struct GraphEdgeSelector *es, int eid) {
    assert(EGEDGE_IT_TYPE_VECTOR == es->type);

    vector_int_push_back(&(es->vec), eid);
    es->end++;
}
/*
 * graph_edge_selector_append - 添加边索引
 *
 * @es: 目标索引器
 * @v: 边索引
 */
void graph_edge_selector_append(struct GraphEdgeSelector *es, const vector_int *v) {
    assert(EGEDGE_IT_TYPE_VECTOR == es->type);

    es->end += vector_int_size(v);
    vector_int_append(v, &(es->vec));
}

