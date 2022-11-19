#pragma once

typedef enum state_vals {
    P_EMPTY=0,
    P_FULL=1
};

typedef uint8_t state_t;

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

Position *position_make(uint32_t);
void position_free(Position *);
state_t position_get_rook(Position *, Vec3);
void position_set_rook(Position *, Vec3, state_t);
Board *board_generate(uint32_t n);
void board_free(Board *board);
