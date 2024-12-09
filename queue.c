#include <stdbool.h>
#include "queue.h"

Queue* create_queue(int capacity) {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->rear = 0;
    queue->data = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}


bool is_queue_empty(Queue *queue) {
    return queue->front == queue->rear;
}


bool is_queue_full(Queue *queue) {
    return (queue->rear + 1) % queue->capacity == queue->front;
}


void enqueue(Queue *queue, int item) {
    if (is_queue_full(queue)) {
        printf("Queue is full\n");
        return;
    }
    queue->data[queue->rear] = item;
    queue->rear = (queue->rear + 1) % queue->capacity;
}


int dequeue(Queue *queue) {
    if (is_queue_empty(queue)) {
        printf("Queue is empty\n");
        return -1;
    }
    int item = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    return item;
}


void free_queue(Queue *queue) {
    free(queue->data);
    free(queue);
}
