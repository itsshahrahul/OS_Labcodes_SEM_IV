#include <stdio.h>
#include <stdlib.h>

int mutex = 1;          // Mutex to control access to the read_count
int write_lock = 1;     // Write lock to ensure mutual exclusion for writers
int read_count = 0;     // Count of readers accessing the shared resource

void reader(int reader_id) {
    --mutex;
    ++read_count;
    if (read_count == 1) {
        --write_lock;   // If this is the first reader, lock the writer
    }
    ++mutex;

    // Reading process
    printf("\nReader %d is reading", reader_id);

    --mutex;
    --read_count;
    if (read_count == 0) {
        ++write_lock;   // If this is the last reader, unlock the writer
    }
    ++mutex;
}

void writer(int writer_id) {
    --write_lock;       // Lock the writer

    // Writing process
    printf("\nWriter %d is writing", writer_id);

    ++write_lock;       // Unlock the writer
}

int main() {
    int n, reader_id = 0, writer_id = 0;
    printf("\n1. Press 1 for Reader"
           "\n2. Press 2 for Writer"
           "\n3. Press 3 for Exit");

    while (1) {
        printf("\nEnter your choice: ");
        scanf("%d", &n);

        // Switch Cases
        switch (n) {
        case 1:
            // If mutex is 1 and write_lock is non-zero, then it is possible to read
            if ((mutex == 1) && (write_lock != 0)) {
                reader(++reader_id);
            }
            // Otherwise, print that readers cannot read
            else {
                printf("\nReaders cannot read!");
            }
            break;

        case 2:
            // If write_lock is 1, then it is possible to write
            if (write_lock == 1) {
                writer(++writer_id);
            }
            // Otherwise, print that writers cannot write
            else {
                printf("\nWriters cannot write!");
            }
            break;

        // Exit Condition
        case 3:
            exit(0);
            break;

        default:
            printf("\nInvalid choice! Please try again.");
            break;
        }
    }
}
