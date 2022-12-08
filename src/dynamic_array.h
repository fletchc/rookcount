#pragma once

typedef struct {
    uint8_t *data;
    size_t unit_size;
    uint32_t capacity;
    uint32_t total;
} DVec;

DVec *vector_make(uint32_t, size_t);
void vector_free(DVec *);
uint32_t vector_add(DVec *, void *);
void vector_set(DVec *, uint32_t, void *);
void *vector_get(DVec *, uint32_t);
void vector_resize(DVec *);
