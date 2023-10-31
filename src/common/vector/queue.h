#pragma once

#include "vector.h"

typedef struct queue_struct {
    Vector base_vector;
    int head;
} Queue;

Queue Queue_Create(size_t elementSize);
void Queue_Reserve(Queue *queue, size_t size);
void Queue_Enqueue(Queue *queue, void* element);

void* Queue_Next(Queue *queue);
void* Queue_Peek(Queue *queue,int index);