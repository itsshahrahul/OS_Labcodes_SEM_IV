#include <stdio.h>
#include <stdbool.h>

#define P 5 // Number of processes
#define R 3 // Number of resources

int available[R]; // Available resources
int max[P][R]; // Maximum demand of each process
int allocation[P][R]; // Resources currently allocated
int need[P][R]; // Resources needed

void calculateNeed() {
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

bool isSafe() {
    int work[R];
    bool finish[P] = {0};
    
    for (int i = 0; i < R; i++) {
        work[i] = available[i];
    }

    while (true) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < R; j++) {
                    if (need[p][j] > work[j]) {
                        break;
                    }
                }
                if (j == R) {
                    for (int k = 0; k < R; k++) {
                        work[k] += allocation[p][k];
                    }
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            break;
        }
    }
    for (int p = 0; p < P; p++) {
        if (!finish[p]) {
            return false;
        }
    }
    return true;
}

bool requestResources(int p, int request[]) {
    for (int i = 0; i < R; i++) {
        if (request[i] > need[p][i]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return false;
        }
        if (request[i] > available[i]) {
            printf("Resources not available. Process must wait.\n");
            return false;
        }
    }

    for (int i = 0; i < R; i++) {
        available[i] -= request[i];
        allocation[p][i] += request[i];
        need[p][i] -= request[i];
    }

    if (isSafe()) {
        printf("Request can be granted.\n");
        return true;
    } else {
        printf("Request cannot be granted as it leads to an unsafe state.\n");
        for (int i = 0; i < R; i++) {
            available[i] += request[i];
            allocation[p][i] -= request[i];
            need[p][i] += request[i];
        }
        return false;
    }
}

int main() {
    printf("Enter the available resources (R1 R2 R3): ");
    for (int i = 0; i < R; i++) {
        scanf("%d", &available[i]);
    }

    printf("Enter the maximum demand for each process:\n");
    for (int i = 0; i < P; i++) {
        printf("Process %d (R1 R2 R3): ", i);
        for (int j = 0; j < R; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("Enter the allocation for each process:\n");
    for (int i = 0; i < P; i++) {
        printf("Process %d (R1 R2 R3): ", i);
        for (int j = 0; j < R; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    calculateNeed();

    int process;
    int request[R];
    printf("Enter the process number making the request: ");
    scanf("%d", &process);

    printf("Enter the request for resources (R1 R2 R3): ");
    for (int i = 0; i < R; i++) {
        scanf("%d", &request[i]);
    }

    if (requestResources(process, request)) {
        printf("Resources allocated to process %d.\n", process);
    } else {
        printf("Process %d must wait.\n", process);
    }

    return 0;
}
