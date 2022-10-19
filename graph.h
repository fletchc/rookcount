#pragma once

typedef enum edge_vals { E_EXISTS, E_EMPTY, E_NONE } edge_t;

typedef struct {
    uint32_t vertex_count;
    AdjMatrix adj_matrix;
} Graph;

Graph *graph_make(uint32_t);
void graph_free(Graph *);
edge_t graph_get_edge(Graph *, uint32_t, uint32_t);
uint32_t graph_set_edge(Graph *, uint32_t, uint32_t, uint8_t);
