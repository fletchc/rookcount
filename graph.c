#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "graph.h"

typedef edge_t **AdjMatrix;

AdjMatrix adj_matrix_make(uint32_t vertex_count) {
    AdjMatrix adj_matrix = calloc(vertex_count, sizeof(uint8_t *));
    if(adj_matrix == NULL) {
        return NULL;
    }

    uint32_t max_edges = ((vertex_count - 1) * vertex_count) / 2;
    uint8_t *adj_matrix_raw = calloc(max_edges, sizeof(uint8_t));

    uint32_t edge_i = 0
    for(uint32_t i = 0; i < vertex_count; i++) {
        adj_matrix[i] = adj_matrix_raw + edge_i;
        edge_i += vertex_count - i - 1;
    }

    return adj_matrix;
}

void adj_matrix_free(Graph *graph) {
    AdjMatrix adj_matrix = graph->adj_matrix;
    free(*adj_matrix);
    free(adj_matrix);
}

Graph *graph_make(uint32_t vertex_count) {
    Graph *graph = calloc(1, sizeof(Graph));
    graph->vertex_count = vertex_count;
    graph->adj_matrix = adj_matrix_make(vertex_count);

    return graph;
}

void graph_free(Graph *graph) {
    adj_matrix_free(graph);
    free(graph);
}

uint8_t graph_validate_edge(Graph *graph, uint32_t *v1, uint32_t *v2) {
    if(v1 >= graph->vertex_count || v2 >= graph->vertex_count || v1 == v2) {
        return -1;
    }
    
    if(v1 > v2) {
        edge_t tmp = *v1;
        *v1 = *v2;
        *v2 = tmp;
    }

    return 0;
}

edge_t graph_get_edge(Graph *graph, uint32_t v1, uint32_t v2) {
    if(graph_validate_edge(graph, &v1, &v2) == -1) {
        return E_NONE;
    }

    return graph->adj_matrix[v1][v2];
}

uint32_t graph_set_edge(Graph *graph, uint32_t v1, uint32_t v2, edge_t edge_val) {
    if(graph_validate_edge(graph, &v1, &v2) == -1) {
        return -1;
    }

    graph->adj_matrix[v1][v2] = edge_val;
    return 0;
}
