#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "diagonal_approx.h"

uint32_t diagonal_approx_compute_min(uint32_t max_n, int64_t flags) {
    Position* main_pos = position_make(max_n, NULL);
    for(uint32_t i = 0; i < max_n; i++) {
        position_fill_diagonal(main_pos, i+1);
        /*position_print(main_pos);*/

        if(switch_active(S_GIVE_LEADING_MINS, flags)) {
            printf("Approximate minimum rook count for size %u board: %u\n", i+1, main_pos->rook_count);
        }
    }

    if(switch_active(S_PRINT_MIN_BOARD, flags)) {
        position_print(main_pos);
    }

    uint32_t count = main_pos->rook_count;
    return count;
}
