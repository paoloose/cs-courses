#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

// similar problem
// https://leetcode.com/problems/find-a-peak-element-ii/description
// MIT 6.006 reference: https://youtu.be/HtSuA80QTyo?t=2155

void rgb(int r, int g, int b);
void rgb_bg(int r, int g, int b);
#define BLACK "\x1b[1;30m"
#define RESET "\x1b[0m"

#define SIZE_ROWS 10
#define SIZE_COLS 10

int** generate_map(int rows, int cols);
int* peak_finding_2d(int** mat, int rows, int cols);
void print_map(int** map, int rows, int cols);
void print_mat(int** map, int start_row, int end_row, int start_col, int end_col);

int main() {
    srand(time(NULL));

    int** map = generate_map(SIZE_ROWS, SIZE_COLS);
    // print_map(map, SIZE_ROWS, SIZE_COLS);
    int* peak = peak_finding_2d(map, SIZE_ROWS, SIZE_COLS);
    printf("Peak found: (%d, %d) -> %d\n", peak[0], peak[1], map[peak[0]][peak[1]]);
}

/*
Algorithm steps to solving the 2d peak finding for a nxm matrix:
- Find the middle column j                          O(1)
- Find the maximun value (i, j) of that column      O(n)
- Pick the left and the right value, if exist       O(1)
- If (i, j) > (i, j-1), (i, j+1), (i, j) is a peak  O(1)
- Else, shrink the matrix with half the columns and repeat

For k repetitions, we will get a time complexity of
    T(n, m) = k * (O(1) + O (n))
In the worst case, we will end up with only one column:
    T(n, 1) = O(n)
That is (m -> m/2 -> m/4 -> m/8 -> ... -> 1), k times
    m/k² = 1
Solving for k, k = log2(m), finally:
    T(n, m) = O(m*log(n))
*/
int* peak_finding_2d(int** mat, int rows, int cols) {
    int* peak_found = malloc(sizeof(int) * 2);
    peak_found[0] = -1;
    peak_found[1] = -1;
    int start_col = 0;
    int end_col = cols - 1;

    while (true) {
        print_mat(mat, 0, rows, start_col, end_col);
        int mid_col = start_col + (end_col - start_col) / 2;

        int max_row = 0;
        for (int i = 0; i < rows; i++) {
            max_row = mat[i][mid_col] > mat[max_row][mid_col] ? i : max_row;
        }
        printf("Finding in the middle column...\n\n");
        print_mat(mat, 0, rows, mid_col, mid_col + 1);
        printf("Found max: (%d, %d) -> %d\n\n", max_row, mid_col, mat[max_row][mid_col]);

        // thanks to the problem conditions, we can say that at least one of the
        // following conditions is true
        bool left_is_bigger  = mid_col != start_col && mat[max_row][mid_col - 1] > mat[max_row][mid_col];
        bool right_is_bigger = mid_col != end_col   && mat[max_row][mid_col + 1] > mat[max_row][mid_col];

        if (!left_is_bigger && !right_is_bigger) {
            peak_found[0] = max_row;
            peak_found[1] = mid_col;
            return peak_found;
        }
        else if (right_is_bigger) {
            printf("It's not a peak. Half to right\n\n");
            start_col = mid_col + 1;
        }
        else if (left_is_bigger) {
            printf("It's not a peak. Half to left\n\n");
            end_col = mid_col - 1;
        }
    }
    return peak_found;
}

int** generate_map(int rows, int cols) {
    int** map = 0;
    map = malloc(sizeof(int*) * rows);
    for (int i = 0; i < rows; i++) {
        map[i] = malloc(sizeof(int) * cols);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            bool is_valid = false;
            do {
                map[i][j] = rand() % 25;
                // left
                if (j != 0 && map[i][j - 1] == map[i][j]) {
                    is_valid = false;
                }
                // up
                else if (i != 0 && map[i - 1][j] == map[i][j]) {
                    is_valid = false;
                }
                // down
                else if (i != rows - 1 && map[i + 1][j] == map[i][j]) {
                    is_valid = false;
                }
                // right
                else if (j != cols - 1 && map[i][j + 1] == map[i][j]) {
                    is_valid = false;
                }
                else {
                    is_valid = true;
                }
            } while (!is_valid);
        }
    }
    return map;
}

void print_mat(int** map, int start_row, int end_row, int start_col, int end_col) {
    for (int i = start_row; i < end_row; i++) {
        for (int j = start_col; j < end_col; j++) {
            rgb_bg(255, 255 - map[i][j] * 10, 15);
            printf(BLACK "%3d", map[i][j]);
        }
        printf(RESET "\n");
    }
    printf("\n");
}

void print_map(int** map, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            rgb_bg(255, 255 - map[i][j] * 10, 15);
            printf(BLACK "%3d", map[i][j]);
        }
        printf(RESET "\n");
    }
}

void rgb(int r, int g, int b) {
    char color[32];
    snprintf(color, 32, "\x1b[38;2;%d;%d;%dm", r, g, b);
    printf("%s", color);
}

void rgb_bg(int r, int g, int b) {
    char color[32];
    snprintf(color, 32, "\x1b[48;2;%d;%d;%dm", r, g, b);
    printf("%s", color);
}
