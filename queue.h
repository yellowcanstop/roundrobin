#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

typedef struct {
    int *data;
    int front;
    int rear;
    int capacity;
} Queue;

Queue* create_queue(int capacity);
bool is_queue_empty(Queue *queue);
bool is_queue_full(Queue *queue);
void enqueue(Queue *queue, int item);
int dequeue(Queue *queue);
void free_queue(Queue *queue);

#endif // QUEUE_H_INCLUDED
