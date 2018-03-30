TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += basic \
    basic/vector \
    basic/queue \
    basic/graph

INCLUDEPATH += tests

SOURCES += main.c \
    basic/queue/queue_uint8.c \
    basic/binary_tree.c \
    basic/dictionary.c \
    inputs.c \
    tests/t_queue_uint8.c \
    tests/t_inputs.c \
    basic/error.c \
    basic/vector/vector_int.c \
    basic/vector/vector_voidptr.c \
    tests/t_vector_int.c \
    basic/graph/graph.c \
    tests/t_graph.c \
    basic/graph/graph_topology.c \
    basic/graph/graph_iterator.c \
    basic/graph/graph_degree.c \
    basic/graph/graph_adjlist.c \
    lexer.c


HEADERS += \
    basic/queue/queue_t_c.h \
    basic/queue/queue_t_h.h \
    basic/queue/queue_uint8.h \
    basic/vector/vector_t_c.h \
    basic/vector/vector_t_h.h \
    basic/vector/vector_t_num_c.h \
    basic/vector/vector_t_num_h.h \
    basic/vector/vector_int.h \
    basic/vector/vector_voidptr.h \
    basic/binary_tree.h \
    basic/dictionary.h \
    basic/mm.h \
    basic/types.h \
    inputs.h \
    tests/t_queue_uint8.h \
    tests/t_inputs.h \
    token.h \
    basic/error.h \
    tests/t_vector_int.h \
    basic/graph/graph.h \
    tests/t_graph.h \
    basic/graph/graph_iterator.h \
    basic/graph/graph_adjlist.h \
    lexer.h
