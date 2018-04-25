TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += basic \
    basic/vector \
    basic/queue \
    basic/graph

INCLUDEPATH += tests src

SOURCES += basic/queue/queue_uint8.c \
    basic/binary_tree.c \
    basic/dictionary.c \
    basic/graph/graph_topology.c \
    basic/graph/graph_iterator.c \
    basic/graph/graph_degree.c \
    basic/graph/graph_adjlist.c \
    basic/error.c \
    basic/vector/vector_int.c \
    basic/vector/vector_voidptr.c \
    basic/graph/graph.c \
    tests/t_queue_uint8.c \
    tests/t_inputs.c \
    tests/t_vector_int.c \
    tests/t_graph.c \
    src/inputs.c \
    src/lexer.c \
    src/main.c \
    basic/filedirectory.c \
    tests/t_dictionary.c \
    tests/t_lexer.c \
    src/parser.c \
    tests/t_parser.c \
    src/symboltable.c \
    src/token.c \
    src/expression.c \
    src/preprocessor.c \
    tests/t_preprocessor.c \
    basic/vector/vector_uint8.c

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
    basic/error.h \
    basic/graph/graph.h \
    basic/graph/graph_iterator.h \
    basic/graph/graph_adjlist.h \
    tests/t_queue_uint8.h \
    tests/t_inputs.h \
    tests/t_vector_int.h \
    tests/t_graph.h \
    src/inputs.h \
    src/lexer.h \
    src/token.h \
    basic/filedirectory.h \
    tests/t_dictionary.h \
    tests/t_lexer.h \
    src/parser.h \
    tests/t_parser.h \
    src/symboltable.h \
    src/expression.h \
    src/preprocessor.h \
    tests/t_preprocessor.h \
    basic/vector/vector_uint8.h

