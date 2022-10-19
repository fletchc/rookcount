#pragma once

typedef uint8_t **AdjMatrix;

typedef struct {
    uint32_t vertex_count;
    AdjMatrix adj_matrix;
} Graph;

Matrix *graph_make(uint32_t);
uint32_t graph_set_edge(uint32_t, uint32_t, uint8_t);
