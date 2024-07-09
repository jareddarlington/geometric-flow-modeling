#include "utils.h"

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