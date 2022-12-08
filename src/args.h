#pragma once

#define SWITCH_COUNT 4
#define PROGRAM_NAME "compute_mins"

// command line argument boilerplate
enum switches {
    S_PRINT_HELP,
    S_GIVE_LEADING_MINS,
    S_PRINT_MIN_BOARD,
    S_USE_DIAG_APPROX
};

int32_t get_switch(char);
int64_t parse_switch(char *);
int8_t switch_active(uint8_t, int64_t);
