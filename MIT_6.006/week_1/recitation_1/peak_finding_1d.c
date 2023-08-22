#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Leetcode reference: https://leetcode.com/problems/find-peak-element
int peak_finding(int* nums, int size);

int main() {
    const int size = 8;
    int nums[8] = { 1, 2, 1, 2, 3, 4, 100000, 1 };
    printf("peak index: %d\n", peak_finding(nums, size));
}

// Returns the index of the first peak encountered. If no peak exists, returns -1
// this is with binary search O(log n)
int peak_finding(int* nums, int size) {
    if (size == 1) return 0; // single element                          // O(1)
    // check for peak at the beginning and the end
    if (nums[0] > nums[1]) return 0;
    if (nums[size - 1] > nums[size - 2]) return size - 1;               // O(1)

    int start = 0;                                                      // O(1)
    int end = size - 1;                                                 // O(1)

    while (start <= end) { // will be repeated log2(n) times for the worst case
        // Any of the following operations inside this loop takes O(1)
        // and they divide the array in half
        int mid = start + ((end - start) / 2);

        bool left_bigger  = nums[mid - 1] > nums[mid];
        bool right_bigger = nums[mid + 1] > nums[mid];

        // Explicit peak:
        if (!left_bigger && !right_bigger) {
            // Note that we have the following constraint:
            //   nums[i] != nums[i + 1] for all valid i.
            // so this must be a peak
            return mid;
        }
        // Section is decreasing
        else if (left_bigger) {
            end = mid - 1;
        }
        // Section is increasing
        else if (right_bigger) {
            start = mid + 1;
        }
        else {
            fprintf(stderr, "this situation is not possible");
            exit(-1);
        }
    }
    return -1;
}
