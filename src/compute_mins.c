#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "compute_mins.h"

static char help_msg[] =
" \
USAGE: [OPTIONS] BOARD_SIZE\n \
Gives the minimum number of non-attacking rooks to cover \
a tetrahedral chessboard of size BOARD_SIZE.\n \
\n \
Options:\n \
-h      Shows this help message.\n \
-g      Prints the minimum number of rooks for every board \
up to size BOARD_SIZE.\n \
-p      Prints the minimal placement found.\n \
-d      Computes an approximate minimal placement by \
filling increasing diagonal slices with rooks.\n \
";

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

    if(switch_active(S_PRINT_HELP, switch_flags)) {
        printf(help_msg);
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
