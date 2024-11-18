#include <windows.h>
#include <stdio.h>

#define MAX_CUSTOMERS 3  // Maximum number of customers the waiting room can hold
#define TOTAL_CUSTOMERS 6 // Total number of customers to simulate

HANDLE mutex;
HANDLE barberReadyEvent;
HANDLE customerReadyEvent;
HANDLE stopEvent;

int waiting = 0;  // Number of customers currently waiting
int barber_ready = 0;
int customers_served = 0;  // Counter for total customers served

DWORD WINAPI barber(LPVOID arg) {
    while (1) {
        WaitForSingleObject(mutex, INFINITE);
        while (waiting == 0 && customers_served < TOTAL_CUSTOMERS) {
            // No customers are waiting and not all customers have been served
            printf("Barber is sleeping...\n");
            ReleaseMutex(mutex);
            WaitForSingleObject(barberReadyEvent, INFINITE);
            WaitForSingleObject(mutex, INFINITE);
        }
        // Check if the simulation should end
        if (customers_served >= TOTAL_CUSTOMERS) {
            ReleaseMutex(mutex);
            SetEvent(stopEvent);  // Signal all customer threads to exit
            return 0;
        }
        // Barber is now ready to cut hair
        barber_ready = 1;
        waiting--;  // One customer gets a haircut
        customers_served++;  // Increment the served customer count
        printf("Barber is cutting hair. Waiting customers: %d\n", waiting);
        SetEvent(customerReadyEvent);  // Notify customer that barber is ready
        ReleaseMutex(mutex);

        // Simulate haircut time
        Sleep(1000);

        WaitForSingleObject(mutex, INFINITE);
        barber_ready = 0;
        ReleaseMutex(mutex);
    }
    return 0;
}

DWORD WINAPI customer(LPVOID arg) {
    int id = *(int*)arg;
    WaitForSingleObject(mutex, INFINITE);
    if (waiting < MAX_CUSTOMERS && customers_served < TOTAL_CUSTOMERS) {
        waiting++;
        printf("Customer %d is waiting. Waiting customers: %d\n", id, waiting);
        SetEvent(barberReadyEvent);  // Notify barber that a customer is waiting
        while (!barber_ready) {
            ReleaseMutex(mutex);
            WaitForSingleObject(customerReadyEvent, INFINITE);
            WaitForSingleObject(mutex, INFINITE);
        }
        printf("Customer %d is getting a haircut.\n", id);
    } else if (customers_served >= TOTAL_CUSTOMERS) {
        printf("Customer %d leaves. The simulation has ended.\n", id);
    } else {
        printf("Customer %d is leaving. No available chairs.\n", id);
    }
    ReleaseMutex(mutex);
    return 0;
}

int main() {
    HANDLE barberThread;
    HANDLE customerThreads[TOTAL_CUSTOMERS];
    int customer_ids[TOTAL_CUSTOMERS];
    
    // Initialize synchronization objects
    mutex = CreateMutex(NULL, FALSE, NULL);
    barberReadyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    customerReadyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    stopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // Create barber thread
    barberThread = CreateThread(NULL, 0, barber, NULL, 0, NULL);

    // Create customer threads
    for (int i = 0; i < TOTAL_CUSTOMERS; i++) {
        customer_ids[i] = i + 1;
        Sleep(rand() % 3000);  // Random arrival time for customers
        customerThreads[i] = CreateThread(
            NULL,
            0,
            customer,
            &customer_ids[i],
            0,
            NULL
        );
    }

    // Wait for all customer threads to finish
    for (int i = 0; i < TOTAL_CUSTOMERS; i++) {
        WaitForSingleObject(customerThreads[i], INFINITE);
        CloseHandle(customerThreads[i]);
    }

    // Signal the barber thread to stop and wait for it to finish
    SetEvent(barberReadyEvent); // Ensure barber wakes up if sleeping
    WaitForSingleObject(stopEvent, INFINITE);
    WaitForSingleObject(barberThread, INFINITE);

    // Clean up
    CloseHandle(barberThread);
    CloseHandle(mutex);
    CloseHandle(barberReadyEvent);
    CloseHandle(customerReadyEvent);
    CloseHandle(stopEvent);

    return 0;
}
