#include "queue.h"
#include "vector.h"
#include <string.h>
#include <stdlib.h>

Queue Queue_Create(size_t elementSize)
{
    Queue ret;
    ret.head = 0;
    ret.base_vector = Vector_Create(elementSize);
    return ret;
}

#define wrap(index, size) ((index) % (size))
#define tail(queue) wrap(queue->head + queue->base_vector.length, queue->base_vector.capacity)

void Queue_Reserve(Queue *queue, size_t size)
{
    void *newData = malloc(size * queue->base_vector.elementSize);
    if (queue->base_vector.length > 0)
    {
        if (tail(queue) <= queue->head)
        {
            memcpy(newData,
                   ((char *)queue->base_vector.pData) + queue->head,
                   (queue->base_vector.capacity - queue->head) * queue->base_vector.elementSize);

            memcpy(((char *)newData) + (queue->base_vector.capacity - queue->head) * queue->base_vector.elementSize,
                   ((char *)queue->base_vector.pData),
                   (tail(queue)-1) * queue->base_vector.elementSize);
        }
        else
        {
            memcpy(newData,
                   ((char *)queue->base_vector.pData) + queue->head,
                   (queue->base_vector.length) * queue->base_vector.elementSize);
        }
    }
    free(queue->base_vector.pData);
    queue->head = 0;

    queue->base_vector.pData = newData;
    queue->base_vector.capacity = size;
}

void Queue_Enqueue(Queue *queue, void *element)
{
    if (queue->base_vector.length >= queue->base_vector.capacity)
    {
        Queue_Reserve(queue, queue->base_vector.capacity * 2);
    }
    memcpy((char *)queue->base_vector.pData + tail(queue) * queue->base_vector.elementSize, element, queue->base_vector.elementSize);
    queue->base_vector.length++;
}

void *Queue_Next(Queue *queue)
{
    void* ret = Vector_At(&queue->base_vector,queue->head);
    queue->base_vector.length--;
    queue->head = wrap(queue->head+1,queue->base_vector.capacity);
    return ret;
}
void *Queue_Peek(Queue *queue, int index)
{
    return Vector_At(&queue->base_vector,wrap(queue->head+index,queue->base_vector.capacity));
}
