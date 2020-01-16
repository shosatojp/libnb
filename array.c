#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int array_init(struct Array* array, int elem_size, int isptr) {
    memset(array, 0, sizeof(struct Array));
    array->isptr = isptr;
    array->elem_size = isptr ? sizeof(void*) : elem_size;
}

int array_expand(struct Array* array, int capacity) {
    void** tmp = (void**)calloc(capacity, array->elem_size);
    if (array->data) {
        memmove(tmp, array->data, (array->length) * array->elem_size);
        free(array->data);
    }
    array->data = tmp;
    array->capacity = capacity;
    return 0;
}

int array_push(struct Array* array, void* e) {
    return array_ins(array, e, array->length);
}

int array_ins(struct Array* array, void* e, int index) {
    if (index > array->length || index < 0) {
        return -1;
    }
    // capacity
    if (array->capacity < array->length + 1) {
        array_expand(array, (array->capacity ? array->capacity : 10) * 2);
    }
    // copy
    memmove(array->data + (index + 1),
            array->data + (index),
            (array->length - index) * array->elem_size);
    memcpy(array->data + index, array->isptr ? &e : e, array->elem_size);
    array->length++;
    return 0;
}

int array_del(struct Array* array, int index) {
    if (index >= array->length || index < 0) {
        return -1;
    }
    memmove(array->data + (index),
            array->data + (index + 1),
            (array->length - index - 1) * array->elem_size);
    memset(array->data + (array->length - 1), 0, 1);
    array->length--;
    return 0;
}

int array_clear(struct Array* array) {
    free(array->data);
}

void* array_at(struct Array* array, int index) {
    return array->isptr ? *array->data : array->data;
}

int main() {
    struct Array arr;
    array_init(&arr, sizeof(struct point*), 1);
    array_expand(&arr, 10);

    struct point p;
    p.x = 1;
    p.y = 1;

    struct point p2;
    p2.x = 1;
    p2.y = 2;

    array_push(&arr, &p);
    array_push(&arr, &p2);
    array_push(&arr, &p2);
    for (int i = 0; i < 100000; i++)
        array_push(&arr, &p);

    array_ins(&arr, &p2, 0);
    array_ins(&arr, &p2, 0);
    array_del(&arr, 3);

    printf("length : %d, capacity : %d\n", arr.length, arr.capacity);
    for (int i = 0; i < arr.length; i++) {
        printf("%lf, %lf\n",
               ((struct point*)(array_at(&arr, i)))->x,
               ((struct point*)(array_at(&arr, i)))->y);
    }
}