#include <stdio.h>
#include <stdlib.h>
#define SIZE 7
#define DISK_SIZE 200
void CSCAN(int arr[], int head)
{
    int seek_count = 0;
    int distance, cur_track;
    int left[SIZE + 2], right[SIZE + 2];
    int left_size = 0, right_size = 0;
    int seek_sequence[SIZE + 2];
    int i, j;
    left[left_size++] = 0;
    right[right_size++] = DISK_SIZE - 1;
    for (i = 0; i < SIZE; i++) {
        if (arr[i] < head)
            left[left_size++] = arr[i];
        if (arr[i] > head)
            right[right_size++] = arr[i];
    }
    for (i = 0; i < left_size - 1; i++) {
        for (j = i + 1; j < left_size; j++) {
            if (left[i] > left[j]) {
                int temp = left[i];
                left[i] = left[j];
                left[j] = temp;
            }
        }
    }
    for (i = 0; i < right_size - 1; i++) {
        for (j = i + 1; j < right_size; j++) {
            if (right[i] > right[j]) {
                int temp = right[i];
                right[i] = right[j];
                right[j] = temp;
            }
        }
    }
    for (i = 0; i < right_size; i++) {
        cur_track = right[i];
        seek_sequence[i] = cur_track;
        distance = abs(cur_track - head);
        seek_count += distance;
        head = cur_track;
    }
    head = 0;
    seek_count += (DISK_SIZE - 1);
    for (i = 0; i < left_size; i++) {
        cur_track = left[i];
        seek_sequence[right_size + i] = cur_track;
        distance = abs(cur_track - head);
        seek_count += distance;
        head = cur_track;
    }
    printf("Total number of seek operations = %d\n", seek_count);
    printf("Seek Sequence is\n");
    for (i = 0; i < right_size + left_size; i++) {
        printf("%d\n", seek_sequence[i]);
    }
}
int main()
{
    int arr[SIZE] = { 82,170,43,140,24,16,190 };
    int head = 50;
    printf("Initial position of head: %d\n", head);
    CSCAN(arr, head);
    return 0;
}
