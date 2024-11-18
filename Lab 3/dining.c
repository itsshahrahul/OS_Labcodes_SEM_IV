#include <windows.h>
#include <stdio.h>

#define NUM_PHILOSOPHERS 3
#define SIMULATION_DURATION 10000 // Duration of simulation in milliseconds (10 seconds)

// Mutexes for each fork
HANDLE forks[NUM_PHILOSOPHERS];
volatile BOOL terminate = FALSE; // Flag to signal termination

DWORD WINAPI philosopher(LPVOID param) {
    int id = *((int*)param);
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    while (!terminate) {
        printf("Philosopher %d is thinking.\n", id);
        Sleep(1000); // Thinking

        // Try to acquire the left fork
        WaitForSingleObject(forks[left], INFINITE);

        // Try to acquire the right fork
        WaitForSingleObject(forks[right], INFINITE);

        // Eating
        printf("Philosopher %d is eating.\n", id);
        Sleep(1000); // Eating

        // Release the right fork
        ReleaseMutex(forks[right]);

        // Release the left fork
        ReleaseMutex(forks[left]);
    }

    return 0;
}

int main() {
    HANDLE threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];
    DWORD startTime = GetTickCount();
    DWORD endTime = startTime + SIMULATION_DURATION;

    // Initialize the mutexes for the forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        forks[i] = CreateMutex(NULL, FALSE, NULL);
        if (forks[i] == NULL) {
            printf("CreateMutex failed for fork %d. Error: %d\n", i, GetLastError());
            return 1;
        }
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        threads[i] = CreateThread(
            NULL,
            0,
            philosopher,
            &ids[i],
            0,
            NULL
        );
        if (threads[i] == NULL) {
            printf("CreateThread failed for philosopher %d. Error: %d\n", i, GetLastError());
            return 1;
        }
    }

    // Run the simulation for a fixed duration
    Sleep(SIMULATION_DURATION);
    terminate = TRUE; // Signal threads to stop

    // Wait for all philosopher threads to finish
    WaitForMultipleObjects(NUM_PHILOSOPHERS, threads, TRUE, INFINITE);

    // Clean up
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        CloseHandle(threads[i]);
        CloseHandle(forks[i]);
    }

    return 0;
}
