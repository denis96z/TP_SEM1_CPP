#include "vector.h"
#include <memory.h>

typedef char byte;

#define VECTOR_LENGTH_INCREMENT 10

int add_to_vector(vector_t *vector, const void *item) {
    if (vector->numItems == vector->length) {
        void *itemsPtr = realloc(vector->items, (vector->length +
                VECTOR_LENGTH_INCREMENT) * vector->itemSize);
        if (!itemsPtr) {
            return 0;
        }

        vector->items = itemsPtr;
        vector->length = vector->length + VECTOR_LENGTH_INCREMENT;
    }

    byte *newItem = (byte*)(vector->items) + (vector->numItems * vector->itemSize);
    memcpy((void*)newItem, item, vector->itemSize);
    ++vector->numItems;

    return 1;
}

int get_from_vector(void *item, vector_t *vector, size_t index) {
    if (index < vector->numItems) {
        byte *itemAtIndex = (byte*)(vector->items) + (index * vector->itemSize);
        memcpy(item, itemAtIndex, vector->itemSize);
        return 1;
    }
    return 0;
}

void remove_all_from_vector(vector_t *vector) {
    free(vector->items);
    vector->items = NULL;
    vector->length = vector->numItems = 0;
}
