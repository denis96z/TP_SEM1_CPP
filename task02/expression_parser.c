#include "expression_parser.h"

#define stack_data_type_t lexem_t
#include "stack.h"

typedef enum {
    LEFT, RIGHT
} associativity_t;

associativity_t get_associativity(const lexem_t *operator);
int get_priority(const lexem_t *operator);

parsing_error_t parse_expression(queue_t *expression, vector_t lexems) {
    stack_t stack;
    init_stack(&stack);

    init_queue(expression);

    lexem_t currentLexem, topStackLexem;
    parsing_error_t result = NO_ERROR;

    for (size_t currentIndex = 0; currentIndex < lexems.numItems; ++currentIndex) {
        get_from_vector(&currentLexem, &lexems, currentIndex);

        if (currentLexem.type == CONSTANT) {
            if (!enqueue(expression, currentLexem)) {
                result = OUT_OF_MEMORY;
                goto on_error;
            }
        }
        else if (currentLexem.type == OPENING_BRACKET) {
            if (!push(&stack, currentLexem)) {
                result = OUT_OF_MEMORY;
                goto on_error;
            }
        }
        else if (currentLexem.type == CLOSING_BRACKET) {
            int openingBracketFound = 0;
            while (stack.numItems && !openingBracketFound) {
                topStackLexem = pop(&stack);
                if (topStackLexem.type == OPENING_BRACKET) {
                    openingBracketFound = 1;
                }
                else {
                    if (!enqueue(expression, topStackLexem)) {
                        result = OUT_OF_MEMORY;
                        goto on_error;
                    }
                }
            }

            if (!openingBracketFound) {
                result = BRACKETS_DISBALANCE;
                goto on_error;
            }
        }
        else if (currentLexem.type == OPERATOR) {
            int currentLexemPriority = get_priority(&currentLexem);
            associativity_t currentLexemAssociativity = get_associativity(&currentLexem);

            while (stack.numItems) {
                topStackLexem = peek(&stack);
                if (topStackLexem.type != OPERATOR) {
                    break;
                }

                int topStackLexemPriority = get_priority(&topStackLexem);

                if ((currentLexemAssociativity == RIGHT &&
                     currentLexemPriority >= topStackLexemPriority) ||
                    (currentLexemAssociativity == LEFT &&
                     currentLexemPriority > topStackLexemPriority)) {
                    break;
                }

                if (!enqueue(expression, pop(&stack))) {
                    result = OUT_OF_MEMORY;
                    goto on_error;
                }
            }

            if (!push(&stack, currentLexem)) {
                result = OUT_OF_MEMORY;
                goto on_error;
            }
        }
    }

    while (stack.numItems) {
        topStackLexem = pop(&stack);
        if (topStackLexem.type != OPERATOR) {
            result = BRACKETS_DISBALANCE;
            goto on_error;
        }
        if (!enqueue(expression, topStackLexem)) {
            result = OUT_OF_MEMORY;
            goto on_error;
        }
    }
    goto on_success;

on_error:
    clear_stack(&stack);
    clear_queue(expression);

on_success:
    return result;
}

associativity_t get_associativity(const lexem_t *operator) {
    switch (operator->value.operator) {
        case '+': case '-': case '*': case '/':
            return LEFT;

        case '#': case '$':
            return LEFT;
    }
}

int get_priority(const lexem_t *operator) {
    switch (operator->value.operator) {
        case '#': case '$':
            return 3;

        case '*': case '/':
            return 2;

        case '+': case '-':
            return 1;
    }
}