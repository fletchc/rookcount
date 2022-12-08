#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "compute_mins.h"

int32_t main(int32_t argc, char *argv[]) {
    int64_t switch_flags = 0;
    uint32_t max_n = 0;
    for(uint32_t i = 1; i < argc; i++) {
        if(argv[i][0] == '-') {
            switch_flags |= parse_switch(argv[i]+1);
        }
        else if(max_n == 0 && atoi(argv[i]) != 0) {
            max_n = atoi(argv[i]);
        }
        else {
            printf("Invalid argument %s. Invoke \"%s -h\" for help. Exiting.\n", argv[i], PROGRAM_NAME);
            return -1;
        }
    }

    /*Position *pos = position_make(2, NULL);*/
    /*Vec3 loc = { 1, 0, 0 };*/
    /*position_set_cell_state(pos, loc, C_OCCUPIED);*/
    /*printf("%u\n", position_fully_attacked(pos, 2));*/
    /*position_print(pos);*/
    /*position_free(pos);*/
    /*return 0;*/

    if(switch_active(S_PRINT_HELP, switch_flags)) {
        printf("\n");
        return 0;
    }

    uint32_t min_rooks;
    if(switch_active(S_USE_DIAG_APPROX, switch_flags)) {
        min_rooks = diagonal_approx_compute_min(max_n, switch_flags);
        if(!switch_active(S_GIVE_LEADING_MINS, switch_flags)) {
            printf("Approximate minimum rook count for size %u board: %u\n", max_n, min_rooks);
        }
    }
    else {
        min_rooks = brute_force_compute_min(max_n, switch_flags);
        if(!switch_active(S_GIVE_LEADING_MINS, switch_flags)) {
            printf("Minimum rook count for size %u board: %u\n", max_n, min_rooks);
        }
    }

    return 0;
}
