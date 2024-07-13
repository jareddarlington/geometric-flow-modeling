#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

// TODO: Abstract delta time functionality into utils

void init(DynamicArray *dynArray, size_t initCap)
{
    dynArray->array = (float *)malloc(initCap * sizeof(float)); // allocate init space
    dynArray->size = 0;                                         // set init size
    dynArray->cap = initCap;                                    // set init cap
}

void push(DynamicArray *dynArray, float value)
{
    if (dynArray->size == dynArray->cap) // if array is at capacity
    {
        dynArray->cap *= 2;                                                                 // double capacity
        dynArray->array = (float *)realloc(dynArray->array, dynArray->cap * sizeof(float)); // allocate more memory
    }

    dynArray->array[dynArray->size++] = value; // add new value
}

void clear(DynamicArray *dynArray)
{
    free(dynArray->array);              // free memory
    dynArray->array = NULL;             // clear array
    dynArray->size = dynArray->cap = 0; // reset size and capacity
}

ssize_t getline(char **lineptr, size_t *n, FILE *stream)
{
    size_t pos;
    int c;

    if (lineptr == NULL || stream == NULL || n == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if (c == EOF)
    {
        return -1;
    }

    if (*lineptr == NULL)
    {
        *lineptr = malloc(128);
        if (*lineptr == NULL)
        {
            return -1;
        }
        *n = 128;
    }

    pos = 0;
    while (c != EOF)
    {
        if (pos + 1 >= *n)
        {
            size_t new_size = *n + (*n >> 2);
            if (new_size < 128)
            {
                new_size = 128;
            }
            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL)
            {
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }

        ((unsigned char *)(*lineptr))[pos++] = c;
        if (c == '\n')
        {
            break;
        }
        c = getc(stream);
    }

    (*lineptr)[pos] = '\0';
    return pos;
}