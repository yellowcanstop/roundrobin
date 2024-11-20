#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 10

/* Define the process structure. */
typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int io_wait_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int status; // -1 = undefined, 0 = ready, 1 = running, 2 = blocked
} Process;

typedef struct Node {
    Process data;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

Node* createNode(Process new_data){
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = new_data;
    new_node->next = NULL;
    return new_node;
}

Queue* createQueue(){
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

int isEmpty(Queue* q){
    if (q->front == NULL && q->rear == NULL) {
        return 1; // is empty
    }
    return 0;
}

// Enqueue will create a new node for the new data
void enqueue(Queue* q, Process new_data)
{
    Node* new_node = createNode(new_data);
    if (q->rear == NULL) {
        q->front = q->rear = new_node;
        return;
    }
    q->rear->next = new_node;
    q->rear = new_node;
}

void dequeue(Queue* q)
{
    if (isEmpty(q)) {
        printf("Queue Underflow\n");
        return;
    }
    Node* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
}


Process processes[MAX_PROCESSES];
int num_processes;
int time_quantum;

// User inputs processes and time quantum
void input_processes() {
    printf("Enter the number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &num_processes);

    for (int i = 0; i < num_processes; i++) {
        printf("Enter details for process %d\n", i + 1);
        processes[i].id = i + 1;
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
        printf("I/O Wait Time: ");
        scanf("%d", &processes[i].io_wait_time);
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].response_time = -1;
        processes[i].status = 0;
    }

    printf("Enter the time quantum: ");
    scanf("%d", &time_quantum);
}

/* Round Robin Scheduling Algorithm */
void round_robin_scheduling() {
    int current_time = 0;
    int completed_process = 0;
    Queue* ready = createQueue();

    for(int i = 0; i < num_processes; i++){
        enqueue(ready, processes[i]);
    }

    do{
        Process temp = ready->front->data;
        printf("%d\t%d\n", temp.id, temp.burst_time);
        temp.burst_time -= time_quantum;
        dequeue(ready);
        if(temp.burst_time > 0){
            enqueue(ready, temp);
        }
    }while(isEmpty(ready) == 0);

}

/* Gantt Chart Function
void print_gantt_chart() {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < gantt_chart_index; i++) {
        if (gantt_chart[i] == -1) {
            printf("| Idle ");
        } else {
            printf("| P%d ", gantt_chart[i]);
        }
    }
    printf("|\n");
}
*/

/* Results Output Function
void print_results() {
    printf("\nProcess ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\tResponse Time\n");
    for (int i = 0; i < num_processes; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].id,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time,
               processes[i].response_time);
    }

    double avg_waiting_time = 0, avg_turnaround_time = 0, avg_response_time = 0;
    for (int i = 0; i < num_processes; i++) {
        avg_waiting_time += processes[i].waiting_time;
        avg_turnaround_time += processes[i].turnaround_time;
        avg_response_time += processes[i].response_time;
    }
    avg_waiting_time /= num_processes;
    avg_turnaround_time /= num_processes;
    avg_response_time /= num_processes;

    printf("\nAverage Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    printf("Average Response Time: %.2f\n", avg_response_time);

    //double cpu_utilization = ((double)total_cpu_busy_time / current_time) * 100;
    //printf("CPU Utilization: %.2f%%\n", cpu_utilization);
}
*/


int main() {
    input_processes();
    round_robin_scheduling();
    return 0;
}