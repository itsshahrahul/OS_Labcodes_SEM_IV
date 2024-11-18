#include <windows.h>
#include <stdio.h>

#define NUM_THREADS 2
#define RUN_TIME 5  // Time (in seconds) after which threads should terminate

// Shared variables
volatile BOOL flag[NUM_THREADS];
volatile int turn;
volatile BOOL running = TRUE;  // Flag to indicate if threads should continue running

// Critical section variables
int counter = 0;

// Handles for thread synchronization
HANDLE threads[NUM_THREADS];
HANDLE mutexHandles[NUM_THREADS]; // Mutexes for critical section synchronization
HANDLE stopEvent;  // Event for signaling termination

// Thread function
DWORD WINAPI process(LPVOID param) {
    int id = *(int*)param;
    int other = (id + 1) % NUM_THREADS;

    while (running) {
        // Set flag and turn
        flag[id] = TRUE;
        turn = other;

        // Wait until it's safe to enter the critical section
        while (flag[other] && turn == other) {
            Sleep(0); // Yield CPU
        }

        // Critical section
        WaitForSingleObject(mutexHandles[id], INFINITE); // Mutex for critical section
        counter++;
        printf("Process %d in critical section. Counter: %d\n", id, counter);
        ReleaseMutex(mutexHandles[id]); // Release mutex after critical section

        // End of critical section
        flag[id] = FALSE;
        
        // Simulate some non-critical section work
        Sleep(500);
    }

    return 0;
}

int main() {
    int ids[NUM_THREADS];

    // Initialize flags, turn, and running state
    flag[0] = flag[1] = FALSE;
    turn = 0;
    running = TRUE;

    // Create event for signaling termination
    stopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (stopEvent == NULL) {
        printf("CreateEvent failed. Error: %d\n", GetLastError());
        return 1;
    }

    // Create mutexes for critical section synchronization
    for (int i = 0; i < NUM_THREADS; i++) {
        mutexHandles[i] = CreateMutex(NULL, FALSE, NULL);
        if (mutexHandles[i] == NULL) {
            printf("CreateMutex failed. Error: %d\n", GetLastError());
            return 1;
        }
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        threads[i] = CreateThread(
            NULL,
            0,
            process,
            &ids[i],
            0,
            NULL
        );
        if (threads[i] == NULL) {
            printf("CreateThread failed for thread %d. Error: %d\n", i, GetLastError());
            return 1;
        }
    }

    // Wait for a certain amount of time before stopping threads
    Sleep(RUN_TIME * 1000);

    // Signal threads to stop
    running = FALSE;

    // Wait for threads to finish
    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

    // Clean up
    for (int i = 0; i < NUM_THREADS; i++) {
        CloseHandle(threads[i]);
        CloseHandle(mutexHandles[i]);
    }
    CloseHandle(stopEvent);

    printf("Threads have finished execution.\n");

    return 0;
}
