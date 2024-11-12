#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define AGING_INTERVAL 5
#define AGING_INCREMENT 1

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
    int io_start_time;
} Process;

Process processes[MAX_PROCESSES];
int num_processes;
int time_quantum;
int gantt_chart[1000]; // Assuming the total time will not exceed 1000 units
int gantt_chart_index = 0;
int total_cpu_busy_time = 0;

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
        processes[i].io_start_time = -1;
    }

    printf("Enter the time quantum: ");
    scanf("%d", &time_quantum);
}

void update_io_wait(int current_time) {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].is_in_io && current_time - processes[i].io_start_time >= processes[i].io_wait_time) {
            processes[i].is_in_io = false;
        }
    }
}

void apply_aging(int current_time) {
    for (int i = 0; i < num_processes; i++) {
        if (!processes[i].is_completed && !processes[i].is_in_io && processes[i].arrival_time <= current_time) {
            processes[i].priority = (processes[i].priority > 1) ? processes[i].priority - AGING_INCREMENT : 1;
        }
    }
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

        // Update I/O wait times
        update_io_wait(current_time);

        // Apply aging
        if (current_time % AGING_INTERVAL == 0) {
            apply_aging(current_time);
        }

        // Process the queue
        if (front < rear) {
            // Find the highest priority process in the queue
            int highest_priority_index = front;
            for (int i = front + 1; i < rear; i++) {
                if (processes[queue[i]].priority < processes[queue[highest_priority_index]].priority) {
                    highest_priority_index = i;
                }
            }

            // Swap the highest priority process to the front
            int temp = queue[front];
            queue[front] = queue[highest_priority_index];
            queue[highest_priority_index] = temp;

            int current_process_index = queue[front++];
            Process *current_process = &processes[current_process_index];

            if (current_process->response_time == -1) {
                current_process->response_time = current_time - current_process->arrival_time;
            }

            int execution_time = (current_process->remaining_time > time_quantum) ? time_quantum : current_process->remaining_time;
            current_process->remaining_time -= execution_time;
            current_time += execution_time;

            // Update Gantt chart and CPU busy time
            for (int t = 0; t < execution_time; t++) {
                gantt_chart[gantt_chart_index++] = current_process->id;
            }
            total_cpu_busy_time += execution_time;

            if (current_process->remaining_time == 0) {
                current_process->is_completed = true;
                current_process->turnaround_time = current_time - current_process->arrival_time;
                current_process->waiting_time = current_process->turnaround_time - current_process->burst_time;
                completed_processes++;
            } else {
                if (current_process->io_wait_time > 0) {
                    current_process->is_in_io = true;
                    current_process->io_start_time = current_time;
                } else {
                    queue[rear++] = current_process_index;
                }
            }
        } else {
            gantt_chart[gantt_chart_index++] = -1; // Idle time
            current_time++;
        }
    }
}

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

    double cpu_utilization = ((double)total_cpu_busy_time / current_time) * 100;
    printf("CPU Utilization: %.2f%%\n", cpu_utilization);
}

int main() {
    input_processes();
    round_robin_scheduling();
    print_gantt_chart();
    print_results();
    return 0;
}