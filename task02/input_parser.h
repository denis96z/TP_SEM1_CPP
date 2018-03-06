#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <stdlib.h>
#include "vector.h"

typedef enum {
    OPERATOR,
    CONSTANT,
    OPENING_BRACKET,
    CLOSING_BRACKET
} lexem_type_t;

typedef struct {
    lexem_type_t type;
    union {
        char operator;
        double constant;
    } value;
} lexem_t;

typedef enum {
    NO_ERROR = 0,
    OUT_OF_MEMORY = 1 << 0,
    ILLEGAL_SYMBOL = 1 << 1,
    BRACKETS_DISBALANCE = 1 << 2
} parsing_error_t;

parsing_error_t parse_input(vector_t *lexems, const char *input);

#endif //INPUT_PARSER_H
