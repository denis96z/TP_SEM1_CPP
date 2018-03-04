#ifndef VECTOR_H
#define VECTOR_H

#define vector_data_type_t int

#ifdef vector_data_type_t

#include <stdlib.h>

#define VECTOR_LENGTH_INCREMENT 10

typedef struct {
    vector_data_type_t *items;
    size_t length;
    size_t numItems;
} vector_t;

void init_vector(vector_t *vector) {
    vector->items = NULL;
    vector->length = vector->numItems = 0;
}

int add_to_vector(vector_t *vector, vector_data_type_t item) {
    if (vector->numItems == vector->length) {
        vector_data_type_t *newItems = (vector_data_type_t*) realloc(vector->items,
            (vector->length + VECTOR_LENGTH_INCREMENT) * sizeof(item));
        if (!newItems) {
            return 0;
        }

        vector->items = newItems;
        vector->length = vector->length + VECTOR_LENGTH_INCREMENT;
    }
    vector->items[vector->numItems++] = item;
}

void clear_vector(vector_t *vector) {
    free(vector->items);
    init_vector(vector);
}

#endif //vector_data_type_t

#endif //VECTOR_H
