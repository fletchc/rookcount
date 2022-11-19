#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "tetboards.h"
#include "graph/graph.h"

Position *position_make(uint32_t n) {
    Position *pos = calloc(1, sizeof(Position));
    if(pos == NULL) return NULL;

    pos->board_size = n;
    pos->rook_count = 0;
    pos->rook_list = calloc((n / 8) + 1, sizeof(uint8_t));
    if(pos->rook_list == NULL) {
        free(pos);
        return NULL;
    }

    return pos;
}

void position_free(Position *pos) {
    free(pos->rook_list);
    free(pos);
}

uint32_t tet(uint32 n) {
    return n*(n + 1)*(n + 2)/6;
}

uint32_t tri(uint32 n) {
    return n*(n + 1)/2;
}

state_t position_get_rook(Position *pos, Vec3 loc) {
    uint32_t loc_i = tet(loc.i) + tri(loc.j) + loc.k;
    return (pos->rook_list[loc_i / 8] >> (loc_i % 8)) & 0x01;
}

void position_set_rook(Position *pos, Vec3 loc, state_t state) {
    uint32_t state_prev = position_get_rook(pos, loc);
    if(state_prev == state) pos->rook_count += state - state_prev;
    uint32_t loc_i = tet(loc.i) + tri(loc.j) + loc.k;
    pos->rook_list[loc_i / 8] |= state << (loc_i % 8);
}

uint8_t position_is_attacked(Board *board, Position *pos, Vec3 loc) {
    if(board == NULL || board->n < pos->board_size) {
        board_free(board);
        board = board_update_size(pos->board_size);
    }
}

Graph *board_generate_attack_graph(uint32_t n) {
    uint32_t vert_count = tet(n);

    Graph *graph = graph_make(vert_count);

    Vec3 *cubic_coords = calloc(vert_count, sizeof(Vec3));
    uint32_t coord_curr;
    for(uint32_t i = 0; i < n; i++) {
        for(uint32_t j = 0; j <= i; j++) {
            for(uint32_t k = 0; k <= j; k++) {
                coord_curr = tet(i) + tri(j) + k;
                cubic_coords[coord_curr].i = j - k;
                cubic_coords[coord_curr].j = k;
                cubic_coords[coord_curr].k = i - j - k;
            }
        }
    }

    for(uint32_t i = 0; i < n; i++) {
        for(uint32_t j = i + 1; j < n; j++) {
            if(!(cubic_coords[i].i - cubic_coords[j].i) +
                    !(cubic_coords[i].j - cubic_coords[j].j) +
                    !(cubic_coords[i].k - cubic_coords[j].k) == 2) {
                graph_set_edge(graph, j, i, E_EXISTS);
            }
        }
    }

    free(cubic_coords);

    return graph;
}

Board *board_generate(uint32_t n) {
    if(n < 1) return NULL;

    Board *board = calloc(1, sizeof(Board));
    if(board == NULL) return NULL;

    board->n = n;
    board->graph = board_generate_attack_graph(n);

    return board;
}

Board *board_update_size(Board *board, uint32_t n) {
    graph_free(board->graph);
    board->graph = board_generate_attack_graph(n);
    board->n = n;

    return board;
}

void board_free(Board *board) {
    graph_free(board->graph);
    free(graph);
}
