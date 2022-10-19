#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "graph.h"

Matrix *graph_make(uint32_t vertex_count);
uint32_t graph_set_edge(uint32_t v1, uint32_t v2, uint8_t edge_val);
