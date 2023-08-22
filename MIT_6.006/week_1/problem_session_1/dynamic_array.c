#include <stdlib.h>
#include <stdio.h>

// din arrays de ints
typedef struct {
    int* data;
    size_t capacity;
    size_t size;
} dinarray;

// returns 0 on success
// if the allocation fails, `arr` will be a 0-sized array with 0 capacity and null data
int init_dinarray(dinarray* arr, int initial_size) {
    if (initial_size < 0) {
        return -69;
    }
    arr->size = 0;
    arr->capacity = 0;
    arr->data = NULL;

    arr->data = calloc(initial_size, sizeof(int));
    if (!arr->data) return -1;
    arr->capacity = initial_size;
    arr->size = initial_size;
    return 0;
}

int dinarray_push(dinarray* arr, int element) {
    if (arr->data == NULL) {
        arr->data = calloc(1, sizeof(int));
        if (!arr->data) return -1;
        arr->data[0] = element;
        return 0;
    }

    // need to resize the data
    if (arr->capacity == arr->size) {
        // arr->capacity can be zero
        size_t new_capacity = arr->capacity > 0 ? arr->capacity * 2 : 1;
        int* tmp = realloc(arr->data, sizeof(int) * new_capacity);
        if (!tmp) return -1;
        arr->data = tmp;
        arr->capacity = new_capacity;
    }
    arr->data[arr->size] = element;
    arr->size++;
    return 0;
}

void dinarray_pop(dinarray* arr) {
    arr->data[arr->size - 1] = 0;
    arr->size--;
}

int dinarray_shrink(dinarray* arr) {
    size_t new_capacity = arr->size;
    int* tmp = realloc(arr->data, sizeof(int) * new_capacity);
    if (!tmp) return -1;
    arr->capacity = new_capacity;
    return 0;
}

int main() {
    dinarray arr;
    init_dinarray(&arr, 1);

    for (int i = 0; i < 100; i++) {
        dinarray_push(&arr, i);
    }

    // dinarray_shrink(&arr);

    printf("capacity: %ld\n", arr.capacity);
    printf("size: %ld\n", arr.size);

    dinarray_pop(&arr);
    dinarray_push(&arr, 69);

    for (size_t i = 0; i < arr.size; i++) {
        printf("%ld: %d\n", i, arr.data[i]);
    }

    printf("new capacity: %ld\n", arr.capacity);
    printf("new size: %ld\n", arr.size);

    return 0;
}
