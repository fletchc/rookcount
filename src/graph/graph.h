#pragma once

typedef enum edge_vals { E_EMPTY, E_EXISTS, E_NONE } edge_t;

typedef edge_t **AdjMatrix;

typedef struct {
    uint32_t vertex_count;
    AdjMatrix adj_matrix;
} Graph;

Graph *graph_make(uint32_t);
void graph_free(Graph *);
edge_t graph_get_edge(Graph *, uint32_t, uint32_t);
uint32_t graph_set_edge(Graph *, uint32_t, uint32_t, edge_t);
void graph_print(Graph *);
