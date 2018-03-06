#include <stdio.h>
#include <stdlib.h>

#include "input_parser.h"
#include "expression_parser.h"
#include "calculator.h"

char* read_expression(void);
char* realloc_buffer(char *buffer, size_t currentSize, size_t newSize);
void free_buffer(char *buffer);

#define PRINT_ERROR printf("[error]");

int main() {
    char *expression = read_expression();
    if (!expression) {
        PRINT_ERROR;
    }

    vector_t lexems;
    if (parse_input(&lexems, expression) != NO_ERROR) {
        free_buffer(expression);
        PRINT_ERROR;
        return 0;
    }

    queue_t expressionQueue;
    if (parse_expression(&expressionQueue, lexems) != NO_ERROR) {
        free_buffer(expression);
        clear_vector(&lexems);
        PRINT_ERROR;
        return 0;
    }

    double result = 0;
    if (calculate(&expressionQueue, &result)) {
        printf("%.2lf", result);
    }
    else {
        PRINT_ERROR;
    }

    clear_queue(&expressionQueue);
    clear_vector(&lexems);
    free_buffer(expression);

    return 0;
}

#define EXPRESSION_BUFFER_LENGTH_INCREMENT 10

char* read_expression(void) {
    char c = '\0';
    char *buffer = NULL;
    size_t bufferLength = 0, expressionLength = 0;

    while (scanf("%c", &c) == 1) {
        if (expressionLength == bufferLength) {
            buffer = realloc_buffer(buffer, bufferLength, bufferLength +
                EXPRESSION_BUFFER_LENGTH_INCREMENT + 1);
            if (!buffer) {
                return NULL;
            }
            bufferLength += EXPRESSION_BUFFER_LENGTH_INCREMENT;
        }

        if (c == '\n') {
            return buffer;
        }

        buffer[expressionLength++] = c;
    }

    return buffer;
}

char* realloc_buffer(char *buffer, size_t currentSize, size_t newSize) {
    char *newBuffer = (char*)realloc(buffer, newSize * sizeof(char));
    if (!newBuffer) {
        free(buffer);
        return NULL;
    }

    for (size_t i = currentSize; i < newSize; ++i) {
        newBuffer[i] = '\0';
    }

    return newBuffer;
}

void free_buffer(char *buffer) {
    free(buffer);
}