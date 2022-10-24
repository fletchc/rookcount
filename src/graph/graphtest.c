#include <stdio.h>
#include <stdint.h>
#include "graph.h"

int main(int32_t argc, int8_t *args[]) {
    Graph *graph = graph_make(3);
    graph_set_edge(graph, 0, 1, E_EXISTS);
    graph_set_edge(graph, 0, 2, E_EXISTS);
    graph_print(graph);
    graph_set_edge(graph, 0, 1, E_EMPTY);
    graph_print(graph);
    graph_free(graph);

    return 0;
}
