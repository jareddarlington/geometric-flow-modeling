#ifndef UTILS_H
#define UTILS_H

#include "stddef.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Function Prototypes
 */

/**
 * @brief Custom getline as Windows doesn't follow POSIX standards. Credit goes to Will Hartung and Antti Haapala.
 *
 * @param lineptr Pointer to the buffer containing the line.
 * @param n       Pointer to the size of the buffer.
 * @param steam   Input stream to read from.
 */
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

#endif