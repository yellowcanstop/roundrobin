#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_TIME_QUANTUM 100 // from Slide 35

typedef struct {
    int *data;
    int front;
    int rear;
    int capacity;
} Queue;

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

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_burst_time;
    int io_wait_time; // specific to each process; amount of time in "blocked" state
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    bool is_completed;
    bool in_queue;
    bool is_ready;
    bool is_running;
    bool is_blocked; // not eligible for CPU scheduling until I/O completes
} Process;

void initialize_processes(Process processes[], int num_processes);
void sort_processes_by_arrival_time(Process processes[], int num_processes);
void sort_processes_by_process_id(Process processes[], int num_processes);
void check_for_new_arrivals(Process processes[], const int num_processes, int *current_time, Queue *ready_queue);
void update_queue(Process processes[], const int num_processes, const int time_quantum, Queue *ready_queue, int *current_time, int *executed_processes);
void round_robin_scheduler(Process processes[], int num_processes, int time_quantum);
void output_results(Process processes[], int num_processes);

int main() {
    int num_processes, time_quantum;

    do {
        printf("Enter number of processes (1-%d): ", MAX_PROCESSES);
        int check_process_count = scanf("%d", &num_processes);
        while (getchar() != '\n'); 
        if (check_process_count != 1 || num_processes < 1 || num_processes > MAX_PROCESSES) {
            printf("Invalid: number of processes needs to be between 1 and %d.\n", MAX_PROCESSES);
        }
    } while (num_processes < 1 || num_processes > MAX_PROCESSES);

    do {
        printf("Enter time quantum (TQ) in milliseconds (at most %d ms): ", MAX_TIME_QUANTUM);
        int check_time_quantum = scanf("%d", &time_quantum);
        while (getchar() != '\n'); 
        if (check_time_quantum != 1 || time_quantum < 0 || time_quantum > MAX_TIME_QUANTUM) {
            printf("Invalid: time quantum needs to be a positive integer and less than %d ms.\n", MAX_TIME_QUANTUM);
        }
    } while (time_quantum < 0 || time_quantum > MAX_TIME_QUANTUM);
    
    Process processes[num_processes + 1];

    initialize_processes(processes, num_processes);

    sort_processes_by_arrival_time(processes, num_processes);

    round_robin_scheduler(processes, num_processes, time_quantum);
    
    output_results(processes, num_processes);

    return 0;
}

void initialize_processes(Process processes[], int num_processes) {
    printf("Enter arrival times, burst times, and I/O wait times (0 if n/a) for each process:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("\nProcess %d: Arrival time = ", i + 1);
        scanf("%d", &processes[i].arrival_time);
        printf("Burst time = ");
        scanf("%d", &processes[i].burst_time);
        printf("I/O wait time = ");
        scanf("%d", &processes[i].io_wait_time);
        processes[i].process_id = i + 1;
        processes[i].remaining_burst_time = processes[i].burst_time;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = -1;
        processes[i].is_completed = false;
        processes[i].in_queue = false;
        processes[i].is_ready = false;
        processes[i].is_running = false;
        processes[i].is_blocked = false;
    }
    printf("\nThank you for your input.\n\n");
}

void sort_processes_by_arrival_time(Process processes[], int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        for (int j = i + 1; j < num_processes; j++) {
            if (processes[i].arrival_time > processes[j].arrival_time) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

void sort_processes_by_process_id(Process processes[], int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        for (int j = i + 1; j < num_processes; j++) {
            if (processes[i].process_id > processes[j].process_id) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
}

void check_for_new_arrivals(Process processes[], const int num_processes, int *current_time, Queue *ready_queue) {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arrival_time <= *current_time && !processes[i].in_queue && !processes[i].is_completed) {
            processes[i].in_queue = true;
            processes[i].is_ready = true;
            enqueue(ready_queue, i);
            printf("Process %d has arrived and is added to the ready queue.\n", processes[i].process_id);
        }
    }
}

void update_queue(Process processes[], const int num_processes, const int time_quantum, Queue *ready_queue, int *current_time, int *executed_processes) {
    printf("Updating queue...\n");
    int current_process = dequeue(ready_queue);
    printf("Process %d is running...\n", processes[current_process].process_id);
    processes[current_process].is_ready = false;
    processes[current_process].is_running = true;
    
    if (processes[current_process].response_time == -1) {
        processes[current_process].response_time = *current_time - processes[current_process].arrival_time;
        printf("Response time for Process %d is %d ms.\n", processes[current_process].process_id, processes[current_process].response_time);
    }

    if (processes[current_process].remaining_burst_time <= time_quantum) {
        processes[current_process].is_running = false;
        processes[current_process].is_completed = true;
        *current_time += processes[current_process].remaining_burst_time;
        processes[current_process].completion_time = *current_time;
        processes[current_process].turnaround_time = processes[current_process].completion_time - processes[current_process].arrival_time;
        processes[current_process].waiting_time = processes[current_process].turnaround_time - processes[current_process].burst_time;
        processes[current_process].remaining_burst_time = 0;
        (*executed_processes)++;

        printf("Process %d has completed execution with completion time of %d ms.\n", processes[current_process].process_id, processes[current_process].completion_time);
        printf("Turnaround time for Process %d is %d ms.\n", processes[current_process].process_id, processes[current_process].turnaround_time);
        printf("Waiting time for Process %d is %d ms.\n", processes[current_process].process_id, processes[current_process].waiting_time);
       
        if (*executed_processes != num_processes) {
            check_for_new_arrivals(processes, num_processes, current_time, ready_queue);
        }
    } else {
        processes[current_process].remaining_burst_time -= time_quantum;
        *current_time += time_quantum;
        processes[current_process].is_running = false;

        printf("Process %d is preempted with %d ms of burst time remaining.\n", processes[current_process].process_id, processes[current_process].remaining_burst_time);

		if (*executed_processes != num_processes) {
            check_for_new_arrivals(processes, num_processes, current_time, ready_queue);
        }
     
		enqueue(ready_queue, current_process);
        processes[current_process].is_ready = true;
        printf("Process %d is added back to the ready queue.\n", processes[current_process].process_id);
    }
}

void round_robin_scheduler(Process processes[], int num_processes, int time_quantum) {
    Queue *ready_queue = create_queue(num_processes + 1);
    enqueue(ready_queue, 0);
    processes[0].in_queue = true;
    processes[0].is_ready = true;

    int current_time = 0; 
    int executed_processes = 0; 

    while (processes[0].arrival_time != current_time) {
        current_time++;
    }

    while (!is_queue_empty(ready_queue)) {
        update_queue(processes, num_processes, time_quantum, ready_queue, &current_time, &executed_processes);
    }

    free_queue(ready_queue);
}

void output_results(Process processes[], int num_processes) {
    double total_turnaround_time = 0;
    double total_waiting_time = 0;
    double total_response_time = 0;
    double total_burst_time = 0;

    sort_processes_by_process_id(processes, num_processes);

    printf("\nProcess ID\tTurnaround Time\tWaiting Time\tResponse Time\n");
    for (int i = 0; i < num_processes; i++) {
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;
        total_response_time += processes[i].response_time;
        total_burst_time += processes[i].burst_time;

        printf("%d\t\t%d\t\t%d\t\t%d\n", processes[i].process_id, processes[i].turnaround_time, processes[i].waiting_time, processes[i].response_time);
    }

    printf("\nAverage Turnaround Time (ms): %.2f\n", total_turnaround_time / num_processes);
    printf("Average Waiting Time (ms): %.2f\n", total_waiting_time / num_processes);
    printf("Average Response Time (ms): %.2f\n", total_response_time / num_processes);
    printf("Total CPU Utilization (%): %.2f%%\n", total_burst_time / (total_burst_time + total_waiting_time) * 100);
}