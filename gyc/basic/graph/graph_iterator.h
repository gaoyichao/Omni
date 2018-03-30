#ifndef GRAPH_ITERATOR_H
#define GRAPH_ITERATOR_H

struct Graph;
struct vector_int;

typedef enum EGraphEdgeITType
{
    EGEDGE_IT_TYPE_SEQ=1,
    EGEDGE_IT_TYPE_VECTOR=2,
} EGraphEdgeITType;


struct GraphEdgeSelector {
    enum EGraphEdgeITType type;
    int pos;
    int start;
    int end;
    vector_int vec;
};

struct GraphEdgeSelector* create_graph_edge_selector(EGraphEdgeITType ittype);
void destroy_graph_edge_selector(struct GraphEdgeSelector *es);

void graph_edge_selector_reset_vector(struct GraphEdgeSelector *es, const vector_int *v);
void graph_edge_selector_reset_sequence(struct GraphEdgeSelector *es, int start, int end);
void graph_edge_selector_push_back(struct GraphEdgeSelector *es, int eid);
void graph_edge_selector_append(struct GraphEdgeSelector *es, const vector_int *v);

#define GRAPH_EIT_NEXT(peit) (++((peit)->pos))
#define GRAPH_EIT_END(peit)   ((peit)->pos >= (peit)->end)
#define GRAPH_EIT_SIZE(peit)  ((peit)->end - (peit)->start)
#define GRAPH_EIT_RESET(peit) ((peit)->pos = (peit)->start)
#define GRAPH_EIT_GET(peit)  \
  ((((peit)->type == EGEDGE_IT_TYPE_SEQ) ? (peit)->pos : VECTOR((peit)->vec)[(peit)->pos]))


#endif // GRAPH_ITERATOR_H
