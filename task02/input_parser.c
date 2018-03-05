#include "input_parser.h"
#include <string.h>

typedef enum {
    INITIAL,
    UN_OPERATION,
    BIN_OPERATION,
    CONST_INT_PART,
    POINT,
    CONST_REAL_PART,
    FINAL
} parser_state_t;

int is_digit(char c);
int is_un_operator(char c);
int is_bin_operator(char c);

parsing_error_t parse_char(vector_t *lexems, parser_state_t *parserState, const char *input);
parsing_error_t add_operator_to_vector(vector_t *lexems, char operator);
parsing_error_t add_unary_operator_to_vector(vector_t *lexems, char operator);
parsing_error_t add_constant_to_vector(vector_t *lexems, const char *constant, size_t constantLength);

parsing_error_t parse_input(vector_t *lexems, const char *input) {
    init_vector(lexems, lexem_t);

    parser_state_t parserState = INITIAL;
    while (*input) {
        parsing_error_t result = parse_char(lexems, &parserState, input);
        if (result != NO_ERROR) {
            clear_vector(lexems);
            return result;
        }
        ++input;
    }

    parserState = FINAL;
    parsing_error_t result = parse_char(lexems, &parserState, input);
    if (result != NO_ERROR) {
        clear_vector(lexems);
        return result;
    }

    return NO_ERROR;
}

parsing_error_t parse_char(vector_t *lexems, parser_state_t *parserState, const char *input) {
    static const char *constant = NULL;
    static size_t constantLength = 0;

    char c = *input;
    switch (*parserState) {
        case INITIAL:
            if (constantLength) {
                parsing_error_t result = add_constant_to_vector(lexems,
                    constant, constantLength) & add_operator_to_vector(lexems, c);
                constantLength = 0;
                if (result != NO_ERROR) {
                    return result;
                }
            }

            if (c == '(') {
                return add_operator_to_vector(lexems, '(');
            }
            else if (is_un_operator(c)) {
                *parserState = UN_OPERATION;
                return add_unary_operator_to_vector(lexems, c);
            }
            else if (is_digit(c)) {
                *parserState = CONST_INT_PART;
                constant = input;
                constantLength = 1;
            }
            else if (c == ' ') {
                //Ignore spaces
            }
            else {
                return ILLEGAL_SYMBOL;
            }
            break;

        case UN_OPERATION:
            if (c == '(') {
                return add_operator_to_vector(lexems, '(');
            }
            else if (is_digit(c)) {
                *parserState = CONST_INT_PART;
                constant = input;
                constantLength = 1;
            }
            else {
                return ILLEGAL_SYMBOL;
            }
            break;

        case CONST_INT_PART:
            if (is_digit(c)) {
                ++constantLength;
            }
            else if (c == '.') {
                *parserState = POINT;
                ++constantLength;
            }
            else if (is_bin_operator(c)) {
                *parserState = BIN_OPERATION;
                parsing_error_t result = add_constant_to_vector(lexems,
                    constant, constantLength) & add_operator_to_vector(lexems, c);
                constantLength = 0;
                return result;
            }
            else {
                return ILLEGAL_SYMBOL;
            }
            break;

        case POINT:
            if (is_digit(c)) {
                *parserState = CONST_REAL_PART;
                ++constantLength;
            }
            else {
                return ILLEGAL_SYMBOL;
            }
            break;

        case CONST_REAL_PART:
            if (is_digit(c)) {
                ++constantLength;
            }
            else if (is_bin_operator(c)) {
                *parserState = BIN_OPERATION;
                parsing_error_t result = add_constant_to_vector(lexems,
                    constant, constantLength) & add_operator_to_vector(lexems, c);
                constantLength = 0;
                return result;
            }
            else if (c == ')') {
                *parserState = INITIAL;
                return add_operator_to_vector(lexems, ')');
            }
            else if (c == ' ') {
                *parserState = INITIAL;
            }
            else {
                return ILLEGAL_SYMBOL;
            }
            break;

        case BIN_OPERATION:
            if (is_digit(c)) {
                *parserState = CONST_INT_PART;
                constant = input;
                constantLength = 1;
            }
            else if (is_un_operator(c)) {
                *parserState = UN_OPERATION;
                return add_unary_operator_to_vector(lexems, c);
            }
            else if (c == '(') {
                *parserState = INITIAL;
                return add_operator_to_vector(lexems, '(');
            }
            else if (c == ' ') {
                *parserState = INITIAL;
            }
            else {
                return ILLEGAL_SYMBOL;
            }
            break;

        case FINAL:
            if (constantLength) {
                return add_constant_to_vector(lexems, constant, constantLength);
            }
    }

    return NO_ERROR;
}

parsing_error_t add_operator_to_vector(vector_t *lexems, char operator) {
    lexem_t lexem;
    lexem.type = OPERATOR;
    lexem.value.operator = operator;
    return add_to_vector(lexems, &lexem) ? NO_ERROR : OUT_OF_MEMORY;
}

parsing_error_t add_unary_operator_to_vector(vector_t *lexems, char operator) {
    // In RPN each operation sign is unique (unary and binary).
    switch (operator) {
        case '+':
            return add_operator_to_vector(lexems, '#');

        case '-':
            return add_operator_to_vector(lexems, '$');

        default:
            return ILLEGAL_SYMBOL;
    }
}

parsing_error_t add_constant_to_vector(vector_t *lexems, const char *constant, size_t constantLength) {
    char buffer[constantLength + 1];
    buffer[constantLength] = '\0';
    strncpy(buffer, constant, constantLength);

    lexem_t lexem;
    lexem.type = CONSTANT;
    lexem.value.constant = atof(buffer);

    return add_to_vector(lexems, &lexem) ? NO_ERROR : OUT_OF_MEMORY;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_un_operator(char c) {
    return c == '+' || c == '-';
}

int is_bin_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}