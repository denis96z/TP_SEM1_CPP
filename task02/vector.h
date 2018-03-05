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

#define init_vector(vectorPtr, type) \
    (vectorPtr)->items = NULL; \
    (vectorPtr)->itemSize = sizeof(type); \
    (vectorPtr)->length = (vectorPtr)->numItems = 0;

#define clear_vector(vectorPtr) \
    remove_all_from_vector(vectorPtr)

#endif //VECTOR_H
