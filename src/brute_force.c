#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "brute_force.h"

typedef struct {
    int8_t *bits;
    uint32_t len;
} BitList;

BitList *bit_list_make(uint32_t len) {
    BitList *list = calloc(1, sizeof(BitList));
    list->bits = calloc(len, sizeof(int8_t));
    list->len = len;
    return list;
}

void bit_list_free(BitList *list) {
    free(list->bits);
    free(list);
}

int8_t bit_list_get_bit(BitList *list, uint32_t i) {
    if(i >= list->len) {
        return -1;
    }
    return list->bits[i];
}

int8_t bit_list_set_bit(BitList *list, int8_t bit, uint32_t i) {
    if(bit != 0 && bit != 1) {
        return -1;
    }
    list->bits[i] = bit;
    return 0;
}

int8_t bit_list_fill(BitList *list, int8_t bit) {
    if(bit != 0 && bit != 1) {
        return -1;
    }
    for(uint32_t i = 0; i < list->len; i++) {
        list->bits[i] = bit;
    }
    return 0;
}

void vector_deep_free_positions(DVec *pos_vector) {
    Position *pos_curr;
    for(uint32_t i = 0; i < pos_vector->total; i++) {
        pos_curr = vector_get(pos_vector, i);
        position_free_rook_list(pos_curr);
    }

    vector_free(pos_vector);
}

Position *vector_find_min(DVec *pos_vector, uint32_t n) {
    Position *pos_min = NULL;
    Position *pos_curr = NULL;
    for(uint32_t i = 0; i < pos_vector->total; i++) {
        pos_curr = vector_get(pos_vector, i);
        if(!position_fully_attacked(pos_curr, n)) continue;

        if(pos_min == NULL || pos_curr->rook_count < pos_min->rook_count) pos_min = pos_curr;
    }

    return pos_curr;
}

uint32_t extend_position_list(Position *pos, DVec *loc_list, DVec *pos_list, BitList *add_list) {
    Position new_pos;
    position_make(pos->board_size, &new_pos);
    position_copy_rooks(&new_pos, pos);
    for(uint32_t i = 0; i < add_list->len; i++) {
        if(!bit_list_get_bit(add_list, i)) continue;
        position_set_cell_state(&new_pos, *(Vec3 *) vector_get(loc_list, i), C_OCCUPIED);
    }

    return vector_add(pos_list, &new_pos);
}

uint32_t enumerate_new_positions(Position *pos, DVec *loc_list, DVec *pos_list, BitList *add_list, uint32_t n, uint32_t curr, uint32_t max_n) {
    if(n == curr) {
        extend_position_list(pos, loc_list, pos_list, add_list);
        return 0;
    }

    bit_list_set_bit(add_list, 0, curr);
    enumerate_new_positions(pos, loc_list, pos_list, add_list, n, curr+1, max_n);
    bit_list_set_bit(add_list, 1, curr);
    enumerate_new_positions(pos, loc_list, pos_list, add_list, n, curr+1, max_n);

    return 0;
}

uint32_t brute_force_compute_min(uint32_t max_n, int64_t flags) {
    // just realized it would be more general and cleaner to start with a size
    // zero board. Oh well.
    Position initial_pos_0;
    Position initial_pos_1;
    initial_pos_0 = *position_make(max_n, &initial_pos_0);
    initial_pos_1 = *position_make(max_n, &initial_pos_1);
    Vec3 initial_pos = { 0, 0, 0 };
    position_set_cell_state(&initial_pos_1, initial_pos, C_OCCUPIED);

    DVec *pos_list_curr = vector_make(2, sizeof(Position));
    vector_add(pos_list_curr, &initial_pos_0);
    vector_add(pos_list_curr, &initial_pos_1);
    DVec *pos_list_prev = NULL;

    DVec *loc_empty_list;
    BitList *bits;

    for(uint32_t i = 1; i < max_n; i++) {
        pos_list_prev = pos_list_curr;
        pos_list_curr = vector_make(pos_list_prev->total, sizeof(Position));

        for(uint32_t pos_i = 0; pos_i < pos_list_prev->total; pos_i++) {
            loc_empty_list = position_get_unattacked_locs(vector_get(pos_list_prev, pos_i), i+1, i);
            if(loc_empty_list->total == 0) {
                Position new_pos;
                position_make(max_n, &new_pos);
                position_copy_rooks(&new_pos, vector_get(pos_list_prev, pos_i));
                vector_add(pos_list_curr, &new_pos);
                vector_free(loc_empty_list);
                continue;
            }
            bits = bit_list_make(loc_empty_list->total);
            enumerate_new_positions(vector_get(pos_list_prev, pos_i), loc_empty_list, pos_list_curr, bits, loc_empty_list->total, 0, max_n);
            bit_list_free(bits);
            vector_free(loc_empty_list);
        }

        if(switch_active(S_GIVE_LEADING_MINS, flags)) {
            printf("Minimum rook count for size %u board: %u\n", i+1, vector_find_min(pos_list_curr, i+1)->rook_count);
        }

        if(pos_list_prev != NULL) {
            vector_deep_free_positions(pos_list_prev);
        }
    }

    Position *min_pos = vector_find_min(pos_list_curr, max_n-1);
    if(switch_active(S_PRINT_MIN_BOARD, flags)) {
        position_print(min_pos);
    }
    uint32_t min_rooks = min_pos->rook_count;
    vector_deep_free_positions(pos_list_curr);


    return min_rooks;
}
