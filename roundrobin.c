#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <iup.h>

#include "queue.h"
#include "robinui.h"
#include "roundrobin.h"

#define MAX_PROCESSES 10
#define MAX_TIME_QUANTUM 100

// Global variables
Process processes[MAX_PROCESSES];
int num_processes;
int time_quantum;

Process gantt_chart[1000]; // Array to store the process execution timeline
int gantt_size = 0;    // Tracks the total timeline size


void fillProcesses(Process processes[], Ihandle* grid){
    printf("PID\tAT\tBT\tIOT\n");
    printf("Time Quantum: %dms\n", time_quantum);
    for(int i = 0; i < num_processes; i++){
        int PID = i+1;
        int AT = getCellValue(grid, PID, 1);
        int BT = getCellValue(grid, PID, 2);
        int IOT = getCellValue(grid, PID, 3);
        processes[i].process_id = PID;
        processes[i].arrival_time = AT;
        processes[i].burst_time = BT;
        processes[i].io_wait_time = IOT;
        processes[i].remaining_burst_time = processes[i].burst_time;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = -1;
        processes[i].blocked_until = -1;
        processes[i].is_completed = false;
        processes[i].in_queue = false;
        processes[i].is_ready = false;
        processes[i].is_running = false;
        processes[i].is_blocked = false;
        printf("%d\t%d\t%d\t%d\n", PID, AT, BT, IOT);
    }
}

int configRoundRobin(Ihandle *self){
    Ihandle *tqVal = IupGetDialogChild(self, "TQINPUT");
    char* timeQuantumStr = IupGetAttribute(tqVal, "VALUE");
    time_quantum = atoi(timeQuantumStr);

    Ihandle* processNumHnd = IupGetDialogChild(self, "processNum");
    char* processNumStr = IupGetAttribute(processNumHnd, "VALUE");
    num_processes = atoi(processNumStr);

    Ihandle* grid = IupGetDialogChild(self, "GRID");
    fillProcesses(processes, grid);
    return IUP_CLOSE;
}

void RoundRobinInput()
{
    Ihandle *mainDialog;
    Ihandle *timeQuantum;
    Ihandle *processNum;
    Ihandle *processNumBtn;
    Ihandle *gridFrame;
    Ihandle *gridbox;
    Ihandle *runBtn;
    processNum = processNumInput();
    processNumBtn = IupGetChild(processNum, 2);
    timeQuantum = timeQuantumInput();

    gridbox = inputGrid();
    gridFrame = IupFrame(gridbox);
    runBtn = IupSetAttributes(IupButton("Run", NULL), "PADDING=3x3");
    mainDialog = IupDialog
    (
    IupVbox
    (
        processNum,
        timeQuantum,
        gridFrame,
        runBtn,
        NULL      // Always end with NULL for this kind of IUP list
    )
    );
    IupSetCallback(processNumBtn, "ACTION", (Icallback)setProcessNum);
    IupSetCallback(runBtn, "ACTION", (Icallback)configRoundRobin);

    IupSetAttribute(mainDialog, "TITLE", "Round Robin Input");
    IupSetAttribute(mainDialog, "MARGIN", "10x10");
    IupSetAttribute(gridFrame, "MARGIN", "0x0");   /* avoid attribute propagation */
    /* Shows dlg in the center of the screen */
    IupShowXY(mainDialog, IUP_CENTER, IUP_CENTER);
}


// Main function
int main(int argc, char **argv) {
    IupOpen(&argc, &argv);
    RoundRobinInput();
    IupMainLoop();
    IupClose();

    sort_processes_by_arrival_time(processes, num_processes);

    printf("\nTime\tProcess ID\tRemaining Time\tStatus");

    round_robin_scheduler();

    output_results();

    print_gantt_chart(gantt_chart,gantt_size);

    return 0;
}


// Helper function for input validation
void check_positive_integer(const char* prompt, int* value) {
    int check;
    do {
        printf("%s", prompt);
        check = scanf("%d", value);
        while (getchar() != '\n');
        if (check != 1 || *value < 0) {
            printf("Invalid: time needs to be a positive integer.\n");
        }
    } while (check != 1 || *value < 0);
}


// Get user input on individual processes
void initialize_processes(Process processes[], int num_processes) {
    printf("Enter arrival times, burst times, and I/O wait times (0 if n/a) for each process:\n");
    printf("Note: For this simulation, I/O operations always happen after the process completes its CPU burst time.\n");
    for (int i = 0; i < num_processes; i++) {
        printf("\nProcess %d:\n", i + 1);
        check_positive_integer("Arrival time = ", &processes[i].arrival_time);
        check_positive_integer("Burst time = ", &processes[i].burst_time);
        check_positive_integer("I/O wait time = ", &processes[i].io_wait_time);
        processes[i].process_id = i + 1;
        processes[i].remaining_burst_time = processes[i].burst_time;
        processes[i].turnaround_time = 0;
        processes[i].waiting_time = 0;
        processes[i].response_time = -1;
        processes[i].blocked_until = -1;
        processes[i].is_completed = false;
        processes[i].in_queue = false;
        processes[i].is_ready = false;
        processes[i].is_running = false;
        processes[i].is_blocked = false;
    }
    printf("\nThank you for your input.\n\n");
}


// Helper function to sort processes by arrival time
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


// Helper function to sort processes by process id
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


// Helper function to check for new arrivals and add them to the ready queue
void check_for_new_arrivals(int *current_time, Queue *ready_queue) {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arrival_time <= *current_time && !processes[i].in_queue && !processes[i].is_completed) {
            processes[i].in_queue = true;
            processes[i].is_ready = true;
            enqueue(ready_queue, i);
            output_process(processes[i].arrival_time, processes[i].process_id, READY, processes[i].remaining_burst_time, processes[i].io_wait_time);
        }
    }
}


// Helper function to log processes in the Gantt chart
void log_to_gantt_chart(Process process, int *current_time) {
    gantt_chart[gantt_size].process_id = process.process_id;
    gantt_chart[gantt_size].arrival_time = *current_time;
    if(time_quantum < process.remaining_burst_time){
        gantt_chart[gantt_size].burst_time = time_quantum;
        gantt_chart[gantt_size].turnaround_time = *current_time + time_quantum;

    }
    else{
        gantt_chart[gantt_size].burst_time = process.remaining_burst_time;
        gantt_chart[gantt_size].turnaround_time = *current_time + process.remaining_burst_time;
    }
    gantt_size++;
}


// Helper function to update the ready queue and status of processes
void update_queue(Queue *ready_queue, int *current_time, int *executed_processes, int *blocked_processes) {
    int current_process = dequeue(ready_queue);
    processes[current_process].is_ready = false;
    processes[current_process].is_running = true;
    output_process(*current_time, processes[current_process].process_id, RUNNING, processes[current_process].remaining_burst_time, processes[current_process].io_wait_time);

    if (processes[current_process].response_time == -1) {
        processes[current_process].response_time = *current_time - processes[current_process].arrival_time;
    }

    if (processes[current_process].remaining_burst_time <= time_quantum) {
        log_to_gantt_chart(processes[current_process], current_time);
        processes[current_process].is_running = false;
        *current_time += processes[current_process].remaining_burst_time;
        processes[current_process].remaining_burst_time = 0;

        if (processes[current_process].io_wait_time > 0) {
            (*blocked_processes)++;
            processes[current_process].is_blocked = true;
            processes[current_process].blocked_until = *current_time + processes[current_process].io_wait_time;
            output_process(*current_time, processes[current_process].process_id, BLOCKED, processes[current_process].remaining_burst_time, processes[current_process].io_wait_time);
        } else {
            processes[current_process].is_completed = true;
            processes[current_process].completion_time = *current_time;
            processes[current_process].turnaround_time = processes[current_process].completion_time - processes[current_process].arrival_time;
            processes[current_process].waiting_time = processes[current_process].turnaround_time - processes[current_process].burst_time;
            (*executed_processes)++;
            output_process(*current_time, processes[current_process].process_id, COMPLETED, processes[current_process].remaining_burst_time, processes[current_process].io_wait_time);
        }

        if (*blocked_processes != 0) {
            check_blocked_processes(current_time, ready_queue, executed_processes, blocked_processes);
        }

        if (*executed_processes != num_processes) {
            check_for_new_arrivals(current_time, ready_queue);
        }
    } else {
        log_to_gantt_chart(processes[current_process], current_time);
        processes[current_process].remaining_burst_time -= time_quantum;
        *current_time += time_quantum;
        processes[current_process].is_running = false;

        if (*blocked_processes != 0) {
            check_blocked_processes(current_time, ready_queue, executed_processes, blocked_processes);
        }

        if (*executed_processes != num_processes) {
            check_for_new_arrivals(current_time, ready_queue);
        }

        enqueue(ready_queue, current_process);
        processes[current_process].is_ready = true;
        output_process(*current_time, processes[current_process].process_id, READY, processes[current_process].remaining_burst_time, processes[current_process].io_wait_time);
    }
}


// Helper function to check if blocked process has completed its I/O operation
void check_blocked_processes(int *current_time, Queue *ready_queue, int *executed_processes, int *blocked_processes) {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].is_blocked && *current_time >= processes[i].blocked_until) {
            processes[i].is_blocked = false;
            processes[i].io_wait_time = 0;
            enqueue(ready_queue, i);
            (*blocked_processes)--;
            processes[i].is_ready = true;
            output_process(*current_time, processes[i].process_id, READY, processes[i].remaining_burst_time, processes[i].io_wait_time);
        }
    }
}


// Core round robin scheduler
void round_robin_scheduler() {
    Queue *ready_queue = create_queue(num_processes + 1);
    enqueue(ready_queue, 0);
    processes[0].in_queue = true;
    processes[0].is_ready = true;
    int count = 0;

    int current_time = 0;
    int executed_processes = 0;
    int blocked_processes = 0;

    while (processes[0].arrival_time != current_time) {
        current_time++;
    }

    output_process(current_time, processes[0].process_id, READY, processes[0].remaining_burst_time, processes[0].io_wait_time);

    while (!is_queue_empty(ready_queue) || blocked_processes > 0) {
        check_blocked_processes(&current_time, ready_queue, &executed_processes, &blocked_processes);
        if (!is_queue_empty(ready_queue)) {
            update_queue(ready_queue, &current_time, &executed_processes, &blocked_processes);
        } else {
            current_time++;
        }
    }

    free_queue(ready_queue);
}


// Helper function to print status of process throughout the round robin scheduling simulation
void output_process(int current_time, int process_id, Status process_status, int remaining_burst_time, int io_wait_time) {
    char *status;
    switch (process_status) {
        case READY:
            status = "READY";
            break;
        case RUNNING:
            status = "RUNNING";
            break;
        case BLOCKED:
            status = "BLOCKED";
            break;
        case COMPLETED:
            status = "COMPLETED";
            break;
    }
    printf("\n%d\t%d\t\t%d\t\t%s", current_time, process_id, remaining_burst_time + io_wait_time, status);
}


// Output the results to console for individual processes and overall system
void output_results() {
    double total_turnaround_time = 0;
    double total_waiting_time = 0;
    double total_response_time = 0;
    double total_burst_time = 0;

    sort_processes_by_process_id(processes, num_processes);

    printf("\n\nProcess ID\tTurnaround Time\tWaiting Time\tResponse Time\n");
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
    printf("Total CPU Utilization (%%): %.2f%%\n", total_burst_time / (total_burst_time + total_waiting_time) * 100);
}


// Output the gantt chart to console
void print_gantt_chart(Process p[], int n)
{
    int i, j;
    int min_length = 1;
    int num_line;
    // print top bar
    printf(" ");
    for(i=0; i<n; i++) {
        if (p[i].burst_time/2 < min_length) num_line = min_length;
        else num_line = p[i].burst_time/2;
        for(j=0; j< num_line; j++) printf("--");
        printf(" ");
    }
    printf("\n|");

    // printing process id in the middle
    for(i=0; i<n; i++) {
        for(j=0; j<(p[i].burst_time / 2) - 1; j++) printf(" ");
        printf("P%d", p[i].process_id);
        for(j=0; j<(p[i].burst_time / 2 ) - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    // printing bottom bar
    for(i=0; i<n; i++) {
        if (p[i].burst_time/2 < min_length) num_line = min_length;
        else num_line = p[i].burst_time/2;
        for(j=0; j< num_line; j++) printf("--");
        printf(" ");
    }
    printf("\n");

    // printing the time line
    printf("%d", p[0].arrival_time);
    for(i=0; i<n; i++) {
        if (p[i].burst_time/2 < min_length) num_line = min_length;
        else num_line = p[i].burst_time/2;
        for(j=0; j< num_line; j++) printf("  ");
        if(p[i].turnaround_time > 9) printf("\b"); // backspace : remove 1 space
        printf("%d", p[i].turnaround_time);

    }
    printf("\n");
}

