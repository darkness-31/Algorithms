#include <stdlib.h>
#include "array.h"

typedef struct Array
{
    Data* data;
    size_t size;
    FFree* free_func; 
} Array;

// create array
Array *array_create(size_t size, FFree f)
{
    Array* arr = (Array*)malloc(sizeof(Array));
    if (!arr) return NULL;
    
    arr->data = (Data*)malloc(size * sizeof(Data));
    if (!arr->data) {
        free(arr);
        return NULL;
    }
    
    arr->size = size;
    arr->free_func = f;
    
    // Initialize all elements to 0
    for (size_t i = 0; i < size; i++) {
        arr->data[i] = (Data)0;
    }
    
    return arr;
}

// delete array, free memory
void array_delete(Array *arr)
{
    if (!arr) return;
    
    if (arr->free_func) {
        for (size_t i = 0; i < arr->size; i++) {
            if (arr->data[i]) {
                arr->free_func((void*)arr->data[i]);
            }
        }
    }
    
    free(arr->data);
    free(arr);
}

// returns specified array element
Data array_get(const Array *arr, size_t index)
{
    if (!arr || index >= arr->size) return (Data)0;
    return arr->data[index];
}

// sets the specified array element to the value
void array_set(Array *arr, size_t index, Data value)
{
    if (!arr || index >= arr->size) return;
    arr->data[index] = value;
}

// returns array size
size_t array_size(const Array *arr)
{
    if (!arr) return 0;
    return arr->size;
}
