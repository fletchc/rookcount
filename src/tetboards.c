#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "tetboards.h"

uint32_t tet(uint32_t n) {
    return n*(n + 1)*(n + 2)/6;
}

uint32_t tri(uint32_t n) {
    return n*(n + 1)/2;
}

// empty is used if space is already available for the Position struct
Position *position_make(uint32_t n, Position *empty) {
    Position *pos;
    if(empty == NULL) {
        pos = calloc(1, sizeof(Position));
        if(pos == NULL) return NULL;
    }
    else {
        pos = empty;
    }

    pos->board_size = n;
    pos->rook_count = 0;
    pos->rook_list = calloc(tet(n), sizeof(uint8_t));
    if(pos->rook_list == NULL) {
        if(empty == NULL) free(pos);
        return NULL;
    }

    return pos;
}

void position_free(Position *pos) {
    free(pos->rook_list);
    free(pos);
}

Position *position_copy_rooks(Position *dest, Position *src) {
    if(src->board_size < dest->board_size) return NULL;

    memcpy(dest->rook_list, src->rook_list, tet(src->board_size));
    dest->rook_count = src->rook_count;

    return dest;
}

// for use ONLY when another function handles deallocation of the pos struct
// or the struct is allocated on the stack
void position_free_rook_list(Position *pos) {
    free(pos->rook_list);
}

Vec3 cubic_to_diag(Vec3 cubic) {
    return (Vec3){ cubic.i + cubic.j + cubic.k, cubic.i + cubic.j, cubic.j };
}

Vec3 diag_to_cubic(Vec3 diag) {
    return (Vec3){ diag.j - diag.k, diag.k, diag.i - diag.j };
}

uint32_t diag_to_i(Vec3 diag) {
    return tet(diag.i) + tri(diag.j) + diag.k;
}

uint32_t cubic_to_i(Vec3 cubic) {
    return diag_to_i(cubic_to_diag(cubic));
}

cell_t position_get_cell_state(Position *pos, Vec3 loc) {
    return pos->rook_list[diag_to_i(loc)];
}

int32_t position_set_cell_state(Position *pos, Vec3 loc, cell_t state) {
    if(state == C_ATTACKED) return -1;

    uint32_t state_prev = position_get_cell_state(pos, loc);
    if(state_prev != state) pos->rook_count += state - state_prev;

    Vec3 loc_cubic = diag_to_cubic(loc);
    uint32_t loc_i = diag_to_i(loc);
    pos->rook_list[loc_i] |= state;

    cell_t is_attacked = pos->rook_list[loc_i] & 0x2;

    Vec3 attacked;
    for(uint32_t i = 0; i < pos->board_size - loc_cubic.j - loc_cubic.k; i++) {
        attacked = (Vec3){ i, loc_cubic.j, loc_cubic.k};
        pos->rook_list[cubic_to_i(attacked)] |= C_ATTACKED;
    }
    for(uint32_t j = 0; j < pos->board_size - loc_cubic.i - loc_cubic.k; j++) {
        attacked = (Vec3){ loc_cubic.i, j, loc_cubic.k};
        pos->rook_list[cubic_to_i(attacked)] |= C_ATTACKED;
    }
    for(uint32_t k = 0; k < pos->board_size - loc_cubic.i - loc_cubic.j; k++) {
        attacked = (Vec3){ loc_cubic.i, loc_cubic.j, k};
        pos->rook_list[cubic_to_i(attacked)] |= C_ATTACKED;
    }

    pos->rook_list[loc_i] &= is_attacked | state;

    return 0;
}

uint8_t position_loc_attacked(Position *pos, Vec3 loc) {
    return pos->rook_list[diag_to_i(loc)] >> 1;
}

uint8_t position_fully_attacked(Position *pos, uint32_t n) {
    if(n == 0) n = pos->board_size;

    for(uint32_t i = 0; i < tet(n); i++) {
        if(!pos->rook_list[i]) return 0;
    }

    return 1;
}

DVec *position_get_unattacked_locs(Position *pos, uint32_t n, uint32_t skip_slice) {
    Vec3 loc_curr;
    DVec *unattacked = vector_make(tri(n), sizeof(Vec3));
    for(uint32_t i = skip_slice; i < n; i++) {
        for(uint32_t j = 0; j <= i; j++) {
            for(uint32_t k = 0; k <= j; k++) {
                loc_curr = (Vec3) { i, j, k };
                if(!position_loc_attacked(pos, loc_curr)) vector_add(unattacked, &loc_curr);
            }
        }
    }

    return unattacked;
}

Position *position_fill_diagonal(Position *pos, uint32_t n) {
    Vec3 loc_curr;
    for(uint32_t j = 0; j < n; j++) {
        for(uint32_t k = 0; k <= j; k++) {
            loc_curr = (Vec3) { n-1, j, k };
            if(!position_loc_attacked(pos, loc_curr)) {
                position_set_cell_state(pos, loc_curr, C_OCCUPIED);
            }
        }
    }

    return pos;
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
    free(board);
}

void print_k_spaces(uint32_t n) {
    for(uint32_t i = 0; i < n; i++) {
        printf(" ");
    }
}

void position_print(Position *pos) {
    Vec3 loc_curr;
    for(uint32_t i = 0; i < pos->board_size; i++) {
        for(uint32_t j = 0; j <= i; j++) {
            print_k_spaces(pos->board_size - j);
            for(uint32_t k = 0; k <= j; k++) {
                loc_curr = (Vec3) { i, j, k };
                printf("%u ", position_get_cell_state(pos, loc_curr));
            }
            printf("\n");
        }
    }
}

void vector_pos_print_all(DVec *vec) {
    for(uint32_t i = 0; i < vec->total; i++) {
        position_print((Position *) vector_get(vec, i));
    }
}
