#include "input_parser.h"
#include <string.h>

typedef enum {
    INITIAL_STATE,
    UN_OPERATOR_STATE,
    BIN_OPERATOR_STATE,
    CONST_INT_PART_STATE,
    POINT_STATE,
    CONST_REAL_PART_STATE,
    OPENING_BRACKET_STATE,
    CLOSING_BRACKET_STATE,
    FINAL_STATE
} parser_state_t;

int is_digit(char c);
int is_un_operator(char c);
int is_bin_operator(char c);
int is_splitter(char c);

parsing_error_t parse_char(vector_t *lexems, parser_state_t *parserState, const char *input);

parsing_error_t add_operator_to_vector(vector_t *lexems, char operator);
parsing_error_t add_unary_operator_to_vector(vector_t *lexems, char operator);
parsing_error_t add_constant_to_vector(vector_t *lexems, const char *constant, size_t constantLength);
parsing_error_t add_bracket_to_vector(vector_t *lexems, char bracket);

parsing_error_t parse_input(vector_t *lexems, const char *input) {
    init_vector(lexems, lexem_t);

    parser_state_t parserState = INITIAL_STATE;
    while (*input) {
        parsing_error_t result = parse_char(lexems, &parserState, input);
        if (result != NO_ERROR) {
            clear_vector(lexems);
            return result;
        }
        ++input;
    }

    parserState = FINAL_STATE;
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
    parsing_error_t result = NO_ERROR;

    switch (*parserState) {
        case INITIAL_STATE:
            if (is_digit(c)) {
                *parserState = CONST_INT_PART_STATE;
                constant = input;
                constantLength = 1;
            }
            else if (is_un_operator(c)) {
                *parserState = UN_OPERATOR_STATE;
                result = add_unary_operator_to_vector(lexems, c);
            }
            else if (c == '(') {
                *parserState = OPENING_BRACKET;
                result = add_bracket_to_vector(lexems, c);
            }
            else if (is_splitter(c)) {
                //Ignore splitter.
            }
            else {
                result = ILLEGAL_SYMBOL;
            }
            break;

        case CONST_INT_PART_STATE:
            if (is_digit(c)) {
                ++constantLength;
            }
            else if (c == '.') {
                *parserState = POINT_STATE;
                ++constantLength;
            }
            else if (is_bin_operator(c)) {
                *parserState = BIN_OPERATOR_STATE;
                if (constantLength) {
                    result = add_constant_to_vector(lexems, constant, constantLength);
                    constantLength = 0;
                }
                if (result == NO_ERROR) {
                    result = add_operator_to_vector(lexems, c);
                }
            }
            else if (c == ')') {
                *parserState = CLOSING_BRACKET_STATE;
                result = add_bracket_to_vector(lexems, c);
            }
            else if (is_splitter(c)) {
                if (constantLength) {
                    result = add_constant_to_vector(lexems, constant, constantLength);
                    constantLength = 0;
                }
            }
            else {
                result = ILLEGAL_SYMBOL;
            }
            break;

        case POINT_STATE:
            if (is_digit(c)) {
                *parserState = CONST_REAL_PART_STATE;
                ++constantLength;
            }
            else {
                result = ILLEGAL_SYMBOL;
            }
            break;

        case CONST_REAL_PART_STATE:
            if (is_digit(c)) {
                ++constantLength;
            }
            else if (is_bin_operator(c)) {
                *parserState = BIN_OPERATOR_STATE;
                if (constantLength) {
                    result = add_constant_to_vector(lexems, constant, constantLength);
                    constantLength = 0;
                }
                if (result == NO_ERROR) {
                    result = add_operator_to_vector(lexems, c);
                }
            }
            else if (c == ')') {
                *parserState = CLOSING_BRACKET_STATE;
                result = add_bracket_to_vector(lexems, c);
            }
            else if (is_splitter(c)) {
                if (constantLength) {
                    result = add_constant_to_vector(lexems, constant, constantLength);
                    constantLength = 0;
                }
            }
            else {
                result = ILLEGAL_SYMBOL;
            }
            break;

        case UN_OPERATOR_STATE:
            if (is_digit(c)) {
                *parserState = CONST_INT_PART_STATE;
                constant = input;
                constantLength = 1;
            }
            else if (c == '(') {
                *parserState = OPENING_BRACKET_STATE;
                result = add_bracket_to_vector(lexems, c);
            }
            else if (is_splitter(c)) {
                //Ignore splitter
            }
            else {
                result = ILLEGAL_SYMBOL;
            }
            break;

            //Not sure bracket will be here, but for now it works
        case BIN_OPERATOR_STATE: case OPENING_BRACKET_STATE:
            if (is_digit(c)) {
                *parserState = CONST_INT_PART_STATE;
                constant = input;
                constantLength = 1;
            }
            else if (is_un_operator(c)) {
                *parserState = UN_OPERATOR_STATE;
                result = add_unary_operator_to_vector(lexems, c);
            }
            else if (c == '(') {
                *parserState = OPENING_BRACKET_STATE;
                result = add_bracket_to_vector(lexems, c);
            }
            else if (is_splitter(c)) {
                //Ignore splitter
            }
            else {
                result = ILLEGAL_SYMBOL;
            }
            break;

        case CLOSING_BRACKET_STATE:
            if (is_bin_operator(c)) {
                *parserState = BIN_OPERATOR_STATE;
                result = add_operator_to_vector(lexems, c);
            }
            else if (is_splitter(c)) {
                //Ignore splitter
            }
            else {
                result = ILLEGAL_SYMBOL;
            }
            break;

        case FINAL_STATE:
            if (constantLength) {
                result = add_constant_to_vector(lexems, constant, constantLength);
            }
            break;
    }

    return result;
}

#define try_add_to_vector(vectorPtr, lexemPtr) \
    add_to_vector((vectorPtr), (lexemPtr)) ? NO_ERROR : OUT_OF_MEMORY

parsing_error_t add_operator_to_vector(vector_t *lexems, char operator) {
    lexem_t lexem;
    lexem.type = OPERATOR;
    lexem.value.operator = operator;
    return try_add_to_vector(lexems, &lexem);
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

    return try_add_to_vector(lexems, &lexem);
}

parsing_error_t add_bracket_to_vector(vector_t *lexems, char bracket) {
    lexem_t lexem;

    switch (bracket) {
        case '(':
            lexem.type = OPENING_BRACKET;
            break;

        case ')':
            lexem.type = CLOSING_BRACKET;
            break;

        //Normally this will not happen:
        default:
            return ILLEGAL_SYMBOL;
    }

    return try_add_to_vector(lexems, &lexem);
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

int is_splitter(char c) {
    return c == ' ';
}