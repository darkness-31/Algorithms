#include <stdlib.h>
#include "vector.h"

typedef struct Vector
{
    Data* data;        
    size_t size;       
    size_t capacity;   
    FFree* free_func;  
} Vector;

Vector *vector_create(FFree f)
{
    Vector* vector = (Vector*)malloc(sizeof(Vector));
    if (!vector) return NULL;
    
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    vector->free_func = f;
    
    return vector;
}

void vector_delete(Vector *vector)
{
    if (!vector) return;
    
    if (vector->free_func) {
        for (size_t i = 0; i < vector->size; i++) {
            if (vector->data[i]) {
                vector->free_func(vector->data[i]);
            }
        }
    }
    
    free(vector->data);
    free(vector);
}

Data vector_get(const Vector *vector, size_t index)
{
    if (!vector || index >= vector->size) return NULL;
    return vector->data[index];
}

void vector_set(Vector *vector, size_t index, Data value)
{
    if (!vector || index >= vector->size) return;
    vector->data[index] = value;
}

size_t vector_size(const Vector *vector)
{
    if (!vector) return 0;
    return vector->size;
}

void vector_resize(Vector *vector, size_t size)
{
    if (!vector) return;
    
    if (size < vector->size && vector->free_func) {
        for (size_t i = size; i < vector->size; i++) {
            if (vector->data[i]) {
                vector->free_func(vector->data[i]);
            }
        }
    }
    
    if (size > vector->capacity) {
        size_t new_capacity = size * 2;
        Data* new_data = (Data*)realloc(vector->data, new_capacity * sizeof(Data));
        if (!new_data) return;
        
        vector->data = new_data;
        vector->capacity = new_capacity;
    }
    
    for (size_t i = vector->size; i < size; i++) {
        vector->data[i] = NULL;
    }
    
    vector->size = size;
}
