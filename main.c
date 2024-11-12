#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 10

/* Define the process structure. */
typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int io_wait_time;
    int priority;
    int waiting_time;
    int turnaround_time;
    int response_time;
    bool is_completed;
    bool is_in_io;
} Process;

Process processes[MAX_PROCESSES];
int num_processes;
int time_quantum;

/* Input mechanism */
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
        processes[i].remaining_time = processes[i].burst_time;
        printf("I/O Wait Time: ");
        scanf("%d", &processes[i].io_wait_time);
        printf("Priority (1-10): ");
        scanf("%d", &processes[i].priority);
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].response_time = -1;
        processes[i].is_completed = false;
        processes[i].is_in_io = false;
    }

    printf("Enter the time quantum: ");
    scanf("%d", &time_quantum);
}

/* Round Robin Scheduling Algorithm */
void round_robin_scheduling() {
    int current_time = 0;
    int completed_processes = 0;
    int queue[MAX_PROCESSES];
    int front = 0, rear = 0;

    while (completed_processes < num_processes) {
        // Add new processes to the queue
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time == current_time && !processes[i].is_completed) {
                queue[rear++] = i;
            }
        }

        // Process the queue
        if (front < rear) {
            int current_process_index = queue[front++];
            Process *current_process = &processes[current_process_index];

            if (current_process->response_time == -1) {
                current_process->response_time = current_time - current_process->arrival_time;
            }

            int execution_time = (current_process->remaining_time > time_quantum) ? time_quantum : current_process->remaining_time;
            current_process->remaining_time -= execution_time;
            current_time += execution_time;

            if (current_process->remaining_time == 0) {
                current_process->is_completed = true;
                current_process->turnaround_time = current_time - current_process->arrival_time;
                current_process->waiting_time = current_process->turnaround_time - current_process->burst_time;
                completed_processes++;
            } else {
                queue[rear++] = current_process_index;
            }
        } else {
            current_time++;
        }
    }
}

/* Output the results */
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
}

int main() {
    input_processes();
    round_robin_scheduling();
    print_results();
    return 0;
}