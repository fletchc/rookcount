#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h> // for log10

#include "graph.h"

uint32_t max_edges_get(uint32_t vertex_count) {
    return ((vertex_count + 1) * vertex_count) / 2;
}

AdjMatrix adj_matrix_make(uint32_t vertex_count) {
    AdjMatrix adj_matrix = calloc(vertex_count, sizeof(edge_t *));
    if(adj_matrix == NULL) return NULL;

    uint32_t max_edges = max_edges_get(vertex_count);
    edge_t *adj_matrix_raw = calloc(max_edges, sizeof(edge_t));
    if(adj_matrix_raw == NULL) {
        free(adj_matrix);
        return NULL;
    }

    uint32_t edge_i = 0;
    for(uint32_t i = 0; i < vertex_count; i++) {
        edge_i += i;
        adj_matrix[i] = adj_matrix_raw + edge_i;
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
    if(graph == NULL) return NULL;
    graph->vertex_count = vertex_count;
    graph->adj_matrix = adj_matrix_make(vertex_count);

    return graph;
}

void graph_free(Graph *graph) {
    adj_matrix_free(graph);
    free(graph);
}

Graph *graph_embed(Graph *graph_sub, uint32_t vertex_count_sup) {
    Graph *graph_sup = graph_make(vertex_count_sup);
    memcpy(graph_sup->adj_matrix[0], graph_sub->adj_matrix[0],
            sizeof(edge_t)*max_edges_get(graph_sub->vertex_count));

    return graph_sup;
}

uint8_t graph_validate_edge(Graph *graph, uint32_t *v1, uint32_t *v2) {
    if(*v1 >= graph->vertex_count || *v2 >= graph->vertex_count) {
        return -1;
    }
    
    if(*v1 < *v2) {
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

int32_t graph_set_edge(Graph *graph, uint32_t v1, uint32_t v2, edge_t edge_val) {
    if(graph_validate_edge(graph, &v1, &v2) == -1) {
        return -1;
    }

    graph->adj_matrix[v1][v2] = edge_val;
    return 0;
}

uint8_t get_digit_count(uint32_t num) {
    return (uint8_t) log10f(num) + 1;
}

void print_n_spaces(uint32_t n) {
    for(uint32_t i = 0; i < n; i++) {
        printf(" ");
    }
}

// TODO: fix spacing for graphs with more than three digits of vertices
void graph_print(Graph *graph) {
    uint32_t v_count = graph->vertex_count;
    uint8_t col_width = 3;

    uint32_t max_spaces = get_digit_count(v_count);
    print_n_spaces(max_spaces + 1);
    uint32_t space_count;
    for(uint32_t i = 0; i < v_count; i++) {
        printf("%u", i);
        space_count = col_width - get_digit_count(i);
        print_n_spaces(space_count);
    }
    printf("\n");

    for(uint32_t i = 0; i < v_count; i++) {
        printf("%u", i);
        print_n_spaces(max_spaces - get_digit_count(i));

        space_count = col_width - 1;
        for(uint32_t j = 0; j < v_count; j++) {
            if(i > j) {
                print_n_spaces(space_count + 1);
            }
            else {
                print_n_spaces(space_count - 1);
                printf("%u ", (uint32_t) graph_get_edge(graph, i, j));
            }
        }
        printf("\n");
    }
}
