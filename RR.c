#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 4
#define TIME_QUANTUM 2

struct process {
int pid; // process ID
int arrival; // arrival time
int burst; // CPU burst time
int remaining; // remaining CPU burst time
int waiting; // waiting time
int turnaround; // turnaround time
};

void simulate_rr(struct process *procs, int n, int max_time) {
int time = 0;
int i, j;
int completed = 0;
int current_pid = -1; // no process running initially
int quantum_count = 0; // count the number of time units a process has executed

while (time < max_time) {
// check for new arrivals
for (i = 0; i < n; i++) {
if (procs[i].arrival == time) {
procs[i].remaining = procs[i].burst;
}
}

// check if current process has completed
if (current_pid >= 0 && procs[current_pid].remaining == 0) {
procs[current_pid].turnaround = time - procs[current_pid].arrival;
completed++;
current_pid = -1;
quantum_count = 0;
}

// check if there are still processes remaining
if (completed == n) {
break;
}

// find the next process to execute
int next_pid = -1;
if (current_pid < 0) {
// if no process is currently running, choose the next available process
for (i = 0; i < n; i++) {
if (procs[i].remaining > 0) {
next_pid = i;
break;
}
}
} else if (quantum_count == TIME_QUANTUM) {
// if current process has run for the full quantum, switch to the next available process
next_pid = -1;
for (i = current_pid + 1; i < n; i++) {
if (procs[i].remaining > 0) {
next_pid = i;
break;
}
}
if (next_pid < 0) {
// if no process is available after current process, wrap around to the beginning
for (i = 0; i < current_pid; i++) {
if (procs[i].remaining > 0) {
next_pid = i;
break;
}
}
}
quantum_count = 0;
} else {
// continue running the current process
next_pid = current_pid;
}

// update waiting times for all waiting processes
for (i = 0; i < n; i++) {
if (i != next_pid && procs[i].remaining > 0) {
procs[i].waiting++;
}
}

// execute the selected process for one time unit
if (next_pid >= 0) {
procs[next_pid].remaining--;
quantum_count++;
current_pid = next_pid;
}

time++;
}
}

int main() {
// seed the random number generator
srand(time(NULL));

// generate processes with random arrival and burst times
struct process procs[MAX_PROCESSES];
int i;
for (i = 0; i < MAX_PROCESSES; i++) {
procs[i].pid = i+1;
procs[i].arrival = rand() % 10; // arrival time between 0 and 9
procs[i].burst = rand() % 10 + 1; // CPU burst time between 1 and 10
procs[i].remaining = procs[i].burst;
procs[i].waiting = 0;
procs[i].turnaround = 0;
}

// run the simulation
simulate_rr(procs, MAX_PROCESSES, 100);

// calculate average waiting time and response time for each process
int avg_waiting[MAX_PROCESSES], avg_response[MAX_PROCESSES];
float total_waiting = 0, total_response = 0;
for (i = 0; i < MAX_PROCESSES; i++) {
avg_waiting[i] = (float) procs[i].waiting - procs[i].burst;
avg_response[i] = (float) procs[i].turnaround - procs[i].burst;
total_waiting += avg_waiting[i];
total_response += avg_response[i];
}

// print the results
printf("Process\tArrival\tBurst\tWaiting\tResponse\n");
for (i = 0; i < MAX_PROCESSES; i++) {
printf("P%d\t%d\t%d\t%d\t%d\n", procs[i].pid, procs[i].arrival, procs[i].burst, avg_waiting[i], avg_response[i]);
}
printf("Average\t\t\t%.2f\t%.2f\n", total_waiting/MAX_PROCESSES, total_response/MAX_PROCESSES);

return 0;
}
