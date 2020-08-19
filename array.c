#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int array_init(struct Array* array, int elem_size, int isptr) {
    memset(array, 0, sizeof(struct Array));
    array->isptr = isptr;
    array->elem_size = isptr ? sizeof(void*) : elem_size;
}

int array_expand(struct Array* array, int capacity) {
    // void** tmp = (void**)calloc(array->elem_size, capacity);
    void** tmp = (void**)malloc(array->elem_size * capacity);
    if (tmp) {
        if (array->data) {
            printf("%d\n", capacity);
            memmove(tmp, array->data, (array->length) * array->elem_size);
            free(array->data);
        }
        array->data = tmp;
        array->capacity = capacity;
        return 0;
    } else {
        // failed to allocate
        return -1;
    }
}

int array_push(struct Array* array, Element e) {
    return array_ins(array, e, array->length);
}

int array_ins(struct Array* array, Element e, int index) {
    if (index > array->length || index < 0) {
        // index out of range
        return -1;
    } else if (array->capacity < array->length + 1 &&
               array_expand(array, (array->capacity ? array->capacity : 10) * 2) < 0) {
        // capacity
        // failed to expand
        return -1;
    } else {
        // copy
        memmove(array->data + (index + 1),
                array->data + (index),
                (array->length - index) * array->elem_size);
        memcpy(array->data + index, array->isptr ? &e : e, array->elem_size);
        array->length++;
        return 0;
    }
}

int array_del(struct Array* array, int index) {
    if (index >= array->length || index < 0) {
        return -1;
    } else {
        memmove(array->data + (index),
                array->data + (index + 1),
                (array->length - index - 1) * array->elem_size);
        memset(array->data + (array->length - 1), 0, 1);
        array->length--;
        return 0;
    }
}

int array_clear(struct Array* array) {
    free(array->data);
}

Element array_at(struct Array* array, int index) {
    void** ptr = array->data + index;
    return array->isptr ? *ptr : ptr;
}

int main() {
    struct Array arr;

    struct timespec f, t;
    timespec_get(&f, TIME_UTC);

    array_init(&arr, sizeof(struct point), 0);
    array_expand(&arr, 128 * 1024 * 1024);

    timespec_get(&t, TIME_UTC);
    long ns;
    long long s;

    if (f.tv_nsec < t.tv_nsec) {
        ns = t.tv_nsec - f.tv_nsec;
        s = t.tv_sec - f.tv_sec;
    } else {
        ns = 1000000000 + t.tv_nsec - f.tv_nsec;
        s = t.tv_sec - f.tv_sec - 1;
    }
    printf("%lld.%ld sec\n", s, ns);

    struct point p;
    p.x = 1;
    p.y = 1;

    struct point p2;
    p2.x = 1;
    p2.y = 2;

    array_push(&arr, &p);
    array_push(&arr, &p2);
    array_push(&arr, &p2);
    for (int i = 0; i < 10000000; i++)
        array_push(&arr, &p);

    array_ins(&arr, &p2, 0);
    array_ins(&arr, &p2, 0);
    array_del(&arr, 3);

    for (int i = 0; i < arr.length; i++) {
        // printf("%d: %lf, %lf\n", i,
        //        ((struct point*)(array_at(&arr, i)))->x,
        //        ((struct point*)(array_at(&arr, i)))->y);
    }
    printf("length : %d, capacity : %d\n", arr.length, arr.capacity);
    array_clear(&arr);
}