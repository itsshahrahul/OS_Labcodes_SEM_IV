#include <stdio.h>
#define MAX_PROCESSES 20

int main() {
    int n, i, j;
    int arrival_time[MAX_PROCESSES], burst_time[MAX_PROCESSES], remaining_time[MAX_PROCESSES];
    int completion_time[MAX_PROCESSES], turnaround_time[MAX_PROCESSES], waiting_time[MAX_PROCESSES];
    int smallest_bt_index, time, total_waiting_time = 0, total_turnaround_time = 0;

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Enter process %d arrival time: ", i+1);
        scanf("%d", &arrival_time[i]);
        printf("Enter process %d burst time: ", i+1);
        scanf("%d", &burst_time[i]);
        remaining_time[i] = burst_time[i];
    }
    int completed_processes = 0;
    for (time = 0; completed_processes < n; time++) {
        smallest_bt_index = -1;
        for (i = 0; i < n; i++) {
            if (arrival_time[i] <= time && remaining_time[i] > 0) {
                if (smallest_bt_index == -1 || remaining_time[i] < remaining_time[smallest_bt_index]) {
                    smallest_bt_index = i;
                }
            }
        }

        if (smallest_bt_index == -1) {
            continue;
        }

        remaining_time[smallest_bt_index]--;

        if (remaining_time[smallest_bt_index] == 0) {
            completed_processes++;
            completion_time[smallest_bt_index] = time + 1;

            waiting_time[smallest_bt_index] = completion_time[smallest_bt_index] - arrival_time[smallest_bt_index] - burst_time[smallest_bt_index];
            if (waiting_time[smallest_bt_index] < 0) {
                waiting_time[smallest_bt_index] = 0;
            }
            turnaround_time[smallest_bt_index] = completion_time[smallest_bt_index] - arrival_time[smallest_bt_index];

            total_waiting_time += waiting_time[smallest_bt_index];
            total_turnaround_time += turnaround_time[smallest_bt_index];
        }
    }
    printf("\nProcess Arrival Time    Burst Time      Completion Time Waiting Time    Turnaround Time\n");
    for (i = 0; i < n; i++) {
        printf("%-8d%-16d%-16d%-16d%-16d%-16d\n", i+1, arrival_time[i], burst_time[i], completion_time[i], waiting_time[i], turnaround_time[i]);
    }

    float avg_waiting_time = (float)total_waiting_time / n;
    float avg_turnaround_time = (float)total_turnaround_time / n;
    printf("\nAverage waiting time = %.2f\n", avg_waiting_time);
    printf("Average turnaround time = %.2f\n", avg_turnaround_time);

    return 0;
}
