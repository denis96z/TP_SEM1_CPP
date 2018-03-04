#ifndef STACK_H
#define STACK_H

#ifdef stack_data_type_t

#include <stdlib.h>
#include <assert.h>

typedef struct stack_node {
    stack_data_type_t data;
    struct stack_node *next;
} stack_node_t;

typedef struct stack {
    stack_node_t *top;
    size_t numItems;
} stack_t;

void init_stack(stack_t *stack) {
    stack->top = NULL;
    stack->numItems = 0;
}

int push(stack_t *stack, stack_data_type_t item) {
    stack_node_t *newNode = (stack_node_t*)malloc(sizeof(stack_node_t));
    if (!newNode) {
        return 0;
    }

    newNode->data = item;
    newNode->next = stack->top;
    stack->top = newNode;
    ++stack->numItems;

    return 1;
}

stack_data_type_t pop(stack_t *stack) {
    assert(stack->top);

    stack_node_t *tempNode = stack->top;
    stack_data_type_t item = tempNode->data;
    stack->top = tempNode->next;
    --stack->numItems;
    free(tempNode);

    return item;
}

int try_pop(stack_t *stack, stack_data_type_t *item) {
    if (stack->top) {
        return 0;
    }

    stack_node_t *tempNode = stack->top;
    *item = tempNode->data;
    stack->top = tempNode->next;
    --stack->numItems;
    free(tempNode);

    return 1;
}

stack_data_type_t peek(const stack_t *stack) {
    assert(stack->top);
    return stack->top->data;
}

int try_peek(const stack_t *stack, stack_data_type_t *item) {
    if (stack->top) {
        *item = stack->top->data;
        return 1;
    }
    return 0;
}

void clear_stack(stack_t *stack) {
    while (stack->top) {
        stack_node_t *tempNode = stack->top;
        stack->top = tempNode->next;
        free(tempNode);
    }
    stack->numItems = 0;
}

#endif //stack_data_type_t

#endif //STACK_H
