#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

// Create a queue
Queue* create_queue(int capacity) {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = queue->rear = 0;
    queue->data = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}


// Check if the queue is empty
bool is_queue_empty(Queue *queue) {
    return queue->front == queue->rear;
}


// Check if the queue is full
bool is_queue_full(Queue *queue) {
    return (queue->rear + 1) % queue->capacity == queue->front;
}


// Add an item to the queue
void enqueue(Queue *queue, int item) {
    if (is_queue_full(queue)) {
        printf("Queue is full\n");
        return;
    }
    queue->data[queue->rear] = item;
    queue->rear = (queue->rear + 1) % queue->capacity;
}


// Remove an item from the queue
int dequeue(Queue *queue) {
    if (is_queue_empty(queue)) {
        printf("Queue is empty\n");
        return -1;
    }
    int item = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    return item;
}


// Free the queue
void free_queue(Queue *queue) {
    free(queue->data);
    free(queue);
}
