#pragma once

#include "graph.h"
#include "dynamic_array.h"

enum cell_states {
    C_UNSEEN=0,
    C_OCCUPIED=1,
    C_ATTACKED=2,
    C_ATT_AND_OCC=3
};
typedef uint8_t cell_t;

typedef struct {
    uint32_t i;
    uint32_t j;
    uint32_t k;
} Vec3;

typedef struct {
    uint32_t n;
    Graph *graph;
} Board;

typedef struct {
    uint32_t board_size;
    uint32_t rook_count;
    uint8_t *rook_list;
    Board *board;
} Position;

Position *position_make(uint32_t, Position *);
void position_free(Position *);
Position *position_copy_rooks(Position *, Position *);
void position_free_rook_list(Position *);
cell_t position_get_cell_state(Position *, Vec3);
int32_t position_set_cell_state(Position *, Vec3, cell_t);
uint8_t position_fully_attacked(Position *, uint32_t);
DVec *position_get_unattacked_locs(Position *, uint32_t, uint32_t);
Position *position_fill_diagonal(Position *, uint32_t);
Board *board_generate(uint32_t);
void board_free(Board *);
void position_print(Position *);
void vector_pos_print_all(DVec *);
