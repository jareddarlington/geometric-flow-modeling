#ifndef UTILS_H
#define UTILS_H

#include "stddef.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    float *array; // array
    size_t size;  // size
    size_t cap;   // capacity
} DynamicArray;

void init(DynamicArray *dynArray, size_t initCap);
void push(DynamicArray *dynArray, float value);
void clear(DynamicArray *dynArray);
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

#endif