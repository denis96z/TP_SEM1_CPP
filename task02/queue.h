#ifndef QUEUE_H
#define QUEUE_H

#ifdef queue_data_type_t

#include <stdlib.h>
#include <assert.h>

typedef struct queue_node {
    queue_data_type_t data;
    struct queue_node *next;
} queue_node_t;

typedef struct queue {
    queue_node_t *head;
    queue_node_t *tail;
    size_t numItems;
} queue_t;

void init_queue(queue_t *queue) {
    queue->head = queue->tail = NULL;
    queue->numItems = 0;
}

int enqueue(queue_t *queue, queue_data_type_t item) {
    queue_node_t *newNode = (queue_node_t*)malloc(sizeof(queue_node_t));
    if (!newNode) {
        return 0;
    }

    newNode->data = item;
    newNode->next = NULL;

    if (queue->numItems) {
        queue->tail->next = newNode;
    } else {
        queue->head = newNode;
    }

    queue->tail = newNode;
    ++queue->numItems;

    return 1;
}

queue_data_type_t dequeue(queue_t *queue) {
    assert(queue->head);

    queue_node_t *tempNode = queue->head;
    queue_data_type_t item = tempNode->data;
    queue->head = tempNode->next;
    free(tempNode);

    if (!queue->head) {
        queue->tail = NULL;
    }

    --queue->numItems;
    return item;
}

queue_data_type_t top(const queue_t *queue) {
    assert(queue->head);
    return queue->head->data;
}

void clear_queue(queue_t *queue) {
    while (queue->head) {
        queue_node_t *tempNode = queue->head;
        queue->head = tempNode->next;
        free(tempNode);
    }
    queue->tail = NULL;
    queue->numItems = 0;
}

#endif //queue_data_type_t

#endif //QUEUE_H
