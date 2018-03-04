#include <stdio.h>

#include "vector.h"

int main() {
    vector_t vector;
    init_vector(&vector);

    for (int i = 0; i < 40; i++) {
        add_to_vector(&vector, i);
    }

    for (int i = 0; i < 20; i++) {
        printf("%d ", vector.items[i]);
    }

    clear_vector(&vector);

    return 0;
}