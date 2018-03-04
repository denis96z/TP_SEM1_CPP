#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct {
    void *items;
    size_t itemSize;
    size_t length;
    size_t numItems;
} vector_t;

int add_to_vector(vector_t *vector, const void *item);
void remove_all_from_vector(vector_t *vector);
int get_from_vector(void *item, vector_t *vector, size_t index);

#define init_vector(vector, type) \
    (vector).items = NULL; \
    (vector).itemSize = sizeof(type); \
    (vector).length = (vector).numItems = 0;

#define clear_vector(vector) \
    remove_all_from_vector(&vector)

#endif //VECTOR_H
