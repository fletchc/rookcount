#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "args.h"

static const char switch_chars[] = {
    'h',
    'g',
    'p',
    'd'
};

int32_t get_switch(char c) {
    for(uint32_t i = 0; i < SWITCH_COUNT; i++) {
        if(switch_chars[i] == c) {
            return i;
        }
    }
    return -1;
}

int64_t parse_switch(char *arg) {
    int32_t switch_val;
    int64_t switches = 0;
    while(*arg) {
        switch_val = get_switch(*arg++);
        if(switch_val == -1) {
            printf("-%s contains invalid switch %c. Invoke \"%s -h\" for help. Exiting.\n", arg, *(arg-1), PROGRAM_NAME);
            return -1;
        }
        else {
            switches |= 1 << switch_val;
        }
    }

    return switches;
}

int8_t switch_active(uint8_t switch_val, int64_t switches) {
    if(switch_val < 0 || switch_val > SWITCH_COUNT) {
        return -1;
    }

    return (switches >> switch_val) & 1;
}
