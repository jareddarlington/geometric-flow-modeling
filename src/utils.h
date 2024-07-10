#ifndef UTILS_H
#define UTILS_H

#include "stddef.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Structures
 */

typedef struct
{
    float *array; // array
    size_t size;  // size
    size_t cap;   // capacity
} DynamicArray;

/*
 * Function Prototypes
 */

/**
 * @brief Initializes a dynamic array.
 *
 * @param dynArray Dynamic array to initialize.
 * @param initCap  Initial capacity of dynamic array.
 */
void init(DynamicArray *dynArray, size_t initCap);

/**
 * @brief Pushes value to dynamic array.
 *
 * @param dynArray Dynamic array to push to.
 * @param value    Value to push.
 */
void push(DynamicArray *dynArray, float value);

/**
 * @brief Clears dynamic array.
 *
 * @param dynArray Dynamic array to clear.
 */
void clear(DynamicArray *dynArray);

/**
 * @brief Custom getline as Windows doesn't follow POSIX standards. Credit goes to Will Hartung and Antti Haapala.
 *
 * @param lineptr Pointer to the buffer containing the line.
 * @param n       Pointer to the size of the buffer.
 * @param steam   Input stream to read from.
 */
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

#endif