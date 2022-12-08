#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_array.h"

DVec *vector_make(uint32_t capacity, size_t unit_size) {
    DVec *vec = calloc(1, sizeof(DVec));
    if(capacity == 0) {
        vec->data = NULL;
    }
    else {
        vec->data = calloc(capacity, unit_size);
    }
    vec->unit_size = unit_size;
    vec->capacity = capacity;
    vec->total = 0;

    return vec;
}
void vector_free(DVec *vec) {
    if(vec->capacity != 0) {
        free(vec->data);
    }
    free(vec);
}

uint32_t vector_add(DVec *vec, void *element) {
    if(vec->total == vec->capacity) {
        vector_resize(vec);
    }
    memcpy((uint8_t *) vec->data + vec->unit_size*vec->total, element, vec->unit_size);
    vec->total++;
    
    return vec->total - 1;
}

void vector_set(DVec *vec, uint32_t ind, void *element) {
    memcpy(vec->data + ind, element, vec->unit_size);
}

void *vector_get(DVec *vec, uint32_t ind) {
    return (void *) ((uint8_t *) vec->data + vec->unit_size*ind);
}

void vector_resize(DVec *vec) {
    vec->capacity *= 2;
    vec->data = realloc(vec->data, vec->unit_size*vec->capacity);
}
