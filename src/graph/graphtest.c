#include <stdio.h>
#include <stdint.h>
#include "graph.h"

int main(int32_t argc, int8_t *args[]) {
    Graph *graph = graph_make(3);
    graph_set_edge(graph, 0, 0, E_EXISTS);
    graph_set_edge(graph, 0, 1, E_EXISTS);
    graph_set_edge(graph, 0, 2, E_EXISTS);
    graph_set_edge(graph, 2, 2, E_EXISTS);
    graph_print(graph);
    Graph *newg = graph_embed(graph, 5);
    graph_print(newg);
    graph_set_edge(newg, 0, 0, E_EMPTY);
    graph_set_edge(newg, 3, 4, E_EXISTS);
    graph_set_edge(graph, 2, 2, E_EMPTY);
    graph_print(graph);
    graph_print(newg);
    graph_free(graph);
    graph_free(newg);

    return 0;
}
