#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_TIME_QUANTUM 100 // from Slide 35

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int io_wait_time; // specific to each process
    int turnaround_time;
    int waiting_time;
    int response_time;
    bool is_completed;
    bool in_queue;
    bool is_ready;
    bool is_running;
    bool is_blocked;
} Process;

void initialize_processes(Process processes[], int num_processes);
void round_robin_scheduling(Process processes[], int num_processes, int time_quantum, int io_wait_time);
void calculate_and_display_metrics(Process processes[], int num_processes);

int main() {
    int num_processes, time_quantum, global_io_wait_time;

    do {
        printf("Enter number of processes (1-%d): ", MAX_PROCESSES);
        int check_process_count = scanf("%d", &num_processes);
        while (getchar() != '\n'); // clear the input buffer
        if (check_process_count != 1 || num_processes < 1 || num_processes > MAX_PROCESSES) {
            printf("Invalid: number of processes needs to be between 1 and %d.\n", MAX_PROCESSES);
        }
    } while (num_processes < 1 || num_processes > MAX_PROCESSES);

    do {
        printf("Enter time quantum (TQ) (at most %d milliseconds): ", MAX_TIME_QUANTUM);
        int check_time_quantum = scanf("%d", &time_quantum);
        while (getchar() != '\n'); // clear the input buffer
        if (check_time_quantum != 1 || time_quantum < 0 || time_quantum > MAX_TIME_QUANTUM) {
            printf("Invalid: time quantum needs to be a positive integer and less than %d milliseconds.\n", MAX_TIME_QUANTUM);
        }
    } while (time_quantum < 0 || time_quantum > MAX_TIME_QUANTUM);

    // check if we are using global i/o wait time
    do {
        printf("Enter I/O wait time: ");
        int check_io_wait_time = scanf("%d", &global_io_wait_time);
        while (getchar() != '\n'); // clear the input buffer
        if (check_io_wait_time != 1 || io_wait_time < 0) {
            printf("Invalid: I/O wait time needs to be a non-negative integer.\n");
        }
    } while (global_io_wait_time < 0);
    
    Process processes[num_processes + 1];

    initialize_processes(processes, num_processes);
    round_robin_scheduling(processes, num_processes, time_quantum, io_wait_time);
    calculate_and_display_metrics(processes, num_processes);

    return 0;
}

void initialize_processes(Process processes[], int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        printf("Enter arrival time, burst time, and I/O wait time for Process %d: ", i + 1);
        scanf("%d %d %d", &processes[i].arrival_time, &processes[i].burst_time, &processes[i].io_wait_time);
        processes[i].process_id = i + 1;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = -1;
        processes[i].is_completed = false;
        processes[i].in_queue = false;
        processes[i].is_ready = false;
        processes[i].is_running = false;
        processes[i].is_blocked = false; // Initialize as not blocked
    }
}

void round_robin_scheduling(Process processes[], int num_processes, int time_quantum) {
    int time = 0;
    int completed_processes = 0;
    int queue[MAX_PROCESSES];
    int front = 0, rear = 0;

    while (completed_processes < num_processes) {
        // Add new processes to the queue based on arrival time
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time == time && !processes[i].is_completed) {
                queue[rear++] = i;
                processes[i].is_ready = true;
                processes[i].in_queue = true;
            }
        }

        if (front < rear) {
            int current_process = queue[front];
            processes[current_process].is_ready = false;
            processes[current_process].is_running = true;

            if (processes[current_process].response_time == -1) {
                processes[current_process].response_time = time - processes[current_process].arrival_time;
            }

            int execution_time = (processes[current_process].remaining_time < time_quantum) ? processes[current_process].remaining_time : time_quantum;
            processes[current_process].remaining_time -= execution_time;
            time += execution_time;

            if (processes[current_process].remaining_time == 0) {
                processes[current_process].is_completed = true;
                processes[current_process].turnaround_time = time - processes[current_process].arrival_time;
                processes[current_process].waiting_time = processes[current_process].turnaround_time - processes[current_process].burst_time;
                completed_processes++;
                front++;
            } else {
                processes[current_process].is_running = false;
                processes[current_process].is_blocked = true; // Block the process for I/O
                queue[rear++] = current_process;
                front++;
            }
        } else {
            time++;
        }

        // Handle I/O wait time
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].is_blocked) {
                processes[i].io_wait_time--;
                if (processes[i].io_wait_time == 0) {
                    processes[i].is_blocked = false;
                    processes[i].is_ready = true;
                    processes[i].in_queue = true;
                    queue[rear++] = i;
                }
            }
        }
    }
}

void calculate_and_display_metrics(Process processes[], int num_processes) {
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int total_burst_time = 0;

    printf("\nProcess ID\tTurnaround Time\tWaiting Time\tResponse Time\n");
    for (int i = 0; i < num_processes; i++) {
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;
        total_response_time += processes[i].response_time;
        total_burst_time += processes[i].burst_time;

        printf("%d\t\t%d\t\t%d\t\t%d\n", processes[i].process_id, processes[i].turnaround_time, processes[i].waiting_time, processes[i].response_time);
    }

    printf("\nAverage Turnaround Time: %.2f\n", (float)total_turnaround_time / num_processes);
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / num_processes);
    printf("Average Response Time: %.2f\n", (float)total_response_time / num_processes);
    printf("CPU Utilization: %.2f%%\n", (float)total_burst_time / (total_burst_time + total_waiting_time) * 100);
}