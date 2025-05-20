#include <stdlib.h>
#include "queue.h"

typedef struct Queue
{
    Data* data;        
    size_t size;       
    size_t capacity;   
    size_t front;      
    FFree* free_func;  
} Queue;

Queue *queue_create(FFree f)
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) return NULL;
    
    queue->data = NULL;
    queue->size = 0;
    queue->capacity = 0;
    queue->front = 0;
    queue->free_func = f;
    
    return queue;
}

void queue_delete(Queue *queue)
{
    if (!queue) return;
    
    if (queue->free_func) {
        for (size_t i = 0; i < queue->size; i++) {
            size_t index = (queue->front + i) % queue->capacity;
            if (queue->data[index]) {
                queue->free_func(queue->data[index]);
            }
        }
    }
    
    free(queue->data);
    free(queue);
}

void queue_insert(Queue *queue, Data data)
{
    if (!queue) return;
    
    // If we need more space, reallocate
    if (queue->size >= queue->capacity) {
        size_t new_capacity = queue->capacity == 0 ? 1 : queue->capacity * 2;
        Data* new_data = (Data*)malloc(new_capacity * sizeof(Data));
        if (!new_data) return;
        
        // Copy existing elements to the new array
        for (size_t i = 0; i < queue->size; i++) {
            size_t old_index = (queue->front + i) % queue->capacity;
            new_data[i] = queue->data[old_index];
        }
        
        free(queue->data);
        queue->data = new_data;
        queue->capacity = new_capacity;
        queue->front = 0;
    }
    
    // Insert the new element
    size_t insert_pos = (queue->front + queue->size) % queue->capacity;
    queue->data[insert_pos] = data;
    queue->size++;
}

Data queue_get(const Queue *queue)
{
    if (!queue || queue_empty(queue)) return NULL;
    return queue->data[queue->front];
}

void queue_remove(Queue *queue)
{
    if (!queue || queue_empty(queue)) return;

    if (queue->free_func && queue->data[queue->front]) {
        queue->free_func(queue->data[queue->front]);
    }
    
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    
    if (queue_empty(queue)) {
        queue->front = 0;
    }
}

bool queue_empty(const Queue *queue)
{
    if (!queue) return true;
    return queue->size == 0;
}
