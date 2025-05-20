#include <stdlib.h>
#include "stack.h"

typedef struct Stack
{
    Data* data;        
    size_t size;       
    size_t capacity;   
    FFree* free_func;  
} Stack;

Stack *stack_create(FFree f)
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) return NULL;
    
    stack->data = NULL;
    stack->size = 0;
    stack->capacity = 0;
    stack->free_func = f;
    
    return stack;
}

void stack_delete(Stack *stack)
{
    if (!stack) return;
    
    if (stack->free_func) {
        for (size_t i = 0; i < stack->size; i++) {
            if (stack->data[i]) {
                stack->free_func(stack->data[i]);
            }
        }
    }
    
    free(stack->data);
    free(stack);
}

void stack_push(Stack *stack, Data data)
{
    if (!stack) return;
    
    if (stack->size >= stack->capacity) {
        size_t new_capacity = stack->capacity == 0 ? 1 : stack->capacity * 2;
        Data* new_data = (Data*)realloc(stack->data, new_capacity * sizeof(Data));
        if (!new_data) return;
        
        stack->data = new_data;
        stack->capacity = new_capacity;
    }
    
    stack->data[stack->size++] = data;
}

Data stack_get(const Stack *stack)
{
    if (!stack || stack_empty(stack)) return NULL;
    return stack->data[stack->size - 1];
}

void stack_pop(Stack *stack)
{
    if (!stack || stack_empty(stack)) return;

    if (stack->free_func && stack->data[stack->size - 1]) {
        stack->free_func(stack->data[stack->size - 1]);
    }
    
    stack->size--;
}

bool stack_empty(const Stack *stack)
{
    if (!stack) return true;
    return stack->size == 0;
}
