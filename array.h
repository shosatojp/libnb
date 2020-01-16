#pragma once

struct point {
    double x;
    double y;
};

typedef void* Element;

struct Array {
    int length;
    int capacity;
    Element* data;
    int elem_size;
    int isptr;
};

int array_init(struct Array* array, int elem_size, int isptr);
int array_expand(struct Array* array, int capacity);
int array_push(struct Array* array, Element e);
int array_ins(struct Array* array, Element e, int index);
int array_del(struct Array* array, int index);