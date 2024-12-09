#ifndef ROUNDROBIN_H_INCLUDED
#define ROUNDROBIN_H_INCLUDED

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int remaining_burst_time;
    int io_wait_time; // I/O operations always happen after the process completes its burst time
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
    int blocked_until;
    bool is_completed;
    bool in_queue;
    bool is_ready;
    bool is_running;
    bool is_blocked;
} Process;

typedef struct {
    int time;
    int process_id;
    int remaining_burst_time;
    int io_wait_time;
    int remaining_time;
    char status[10]; // Store status like "READY", "RUNNING", "BLOCKED", "COMPLETED"
} ProcessStatusLog;

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    COMPLETED
} Status;

void check_positive_integer(const char* prompt, int* value);
void initialize_processes(Process processes[], int num_processes);
void sort_processes_by_arrival_time(Process processes[], int num_processes);
void sort_processes_by_process_id(Process processes[], int num_processes);
void check_for_new_arrivals(int *current_time, Queue *ready_queue);
void check_blocked_processes(int *current_time, Queue *ready_queue, int *executed_processes, int *blocked_processes);
void update_queue(Queue *ready_queue, int *current_time, int *executed_processes, int *blocked_processes);
void round_robin_scheduler();
void output_process(int current_time, int process_id, Status process_status, int remaining_burst_time, int io_wait_time);
void output_results();
void print_gantt_chart(Process p[], int n);
void export_analysis_to_file();

#endif // ROUNDROBIN_H_INCLUDED
