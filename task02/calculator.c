#include "calculator.h"

#define stack_data_type_t lexem_t
#include "stack.h"

int apply_operator(stack_t *stack, const lexem_t *operator);

int calculate(queue_t *expression, double *result) {
    stack_t stack;
    init_stack(&stack);

    while (expression->numItems) {
        lexem_t currentLexem = dequeue(expression);
        if (currentLexem.type == CONSTANT) {
            if (!push(&stack, currentLexem)) {
                clear_stack(&stack);
                return 0;
            }
        }
        else if (currentLexem.type == OPERATOR) {
            if (!apply_operator(&stack, &currentLexem)) {
                clear_stack(&stack);
                return 0;
            }
        }
    }

    assert(stack.numItems == 1);
    *result = pop(&stack).value.constant;

    return 1;
}

typedef double (*bin_operation_t)(double, double);
double add(double firstConst, double secondConst);
double sub(double firstConst, double secondConst);
double mul(double firstConst, double secondConst);
double dvd(double firstConst, double secondConst);

typedef double (*un_operation_t)(double);
double not(double value);
double self(double value);

int perform_bin_operation(stack_t *stack, bin_operation_t operation);
int perform_un_operation(stack_t *stack, un_operation_t operation);

int apply_operator(stack_t *stack, const lexem_t *operator) {
    switch (operator->value.operator) {
        case '+':
            return perform_bin_operation(stack, add);

        case '-':
            return perform_bin_operation(stack, sub);

        case '*':
            return perform_bin_operation(stack, mul);

        case '/':
            return perform_bin_operation(stack, dvd);

        case '#':
            return perform_un_operation(stack, self);

        case '$':
            return perform_un_operation(stack, not);

        //Normally this will not happen
        default:
            return 0;
    }
}

int perform_bin_operation(stack_t *stack, bin_operation_t operation) {
    double firstConst = pop(stack).value.constant;
    double secondConst = pop(stack).value.constant;
    lexem_t result = {
            .type = CONSTANT,
            .value.constant = operation(secondConst, firstConst)
    };
    if (!push(stack, result)) {
        return 0;
    }
    return 1;
}

int perform_un_operation(stack_t *stack, un_operation_t operation) {
    double value = pop(stack).value.constant;
    lexem_t result = {
            .type = CONSTANT,
            .value.constant = operation(value)
    };
    if (!push(stack, result)) {
        return 0;
    }
    return 1;
}

double add(double firstConst, double secondConst) {
    return firstConst + secondConst;
}

double sub(double firstConst, double secondConst) {
    return firstConst - secondConst;
}

double mul(double firstConst, double secondConst) {
    return firstConst * secondConst;
}

double dvd(double firstConst, double secondConst) {
    return firstConst / secondConst;
}

double not(double value) {
    return -value;
}

double self(double value) {
    return value;
}
