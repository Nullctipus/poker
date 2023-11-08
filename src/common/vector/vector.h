#pragma once

#include <stdlib.h>
typedef struct Vector {
  void *pData;
  unsigned long long length;
  unsigned long long capacity;
  size_t elementSize;
} Vector;

/*
 * @breif Create a vector setting element size and allocating 2 spaces
 * @param elementSize Declare size of an element in array
 */
Vector Vector_Create(size_t elementSize);
/*
 * @breif Copy a vectors values to a new vector
 * @param vector Vector to copy
 */
Vector Vector_Copy(Vector *vector);
/*
 * @breif Get pointer to object at index
 * @param vector Vector to read
 * @param index Index of data to read
 */
void *Vector_At(Vector *vector, unsigned long long index);
/*
 * @breif Append an element to a vector
 * @param vector Vector to manipulate
 * @param element Element to append
 */
void *Vector_PushBack(Vector *vector, const void *element);
/*
 * @breif Remove the last element in vector
 * @param vector Vector to manipulate
 * @returns element added
 */
void Vector_Pop(Vector *vector);
/*
 * @breif Free data
 * @param vector Vector to free
 */
void Vector_Dispose(Vector *vector);
/*
 * @breif Remove element at index
 * @param vector Vector to manipulate
 * @param index Index of element to remove
 * @param bOrdered Whether or not to keep order of array
 */
void Vector_RemoveAt(Vector *vector, unsigned long long index, int bOrdered);
/*
 * @breif Allocate an amount of space before pushing
 * @param vector Vector to manipulate
 * @param size New size of vector
 */
void Vector_Reserve(Vector *vector, size_t size);
/*
 * @breif Allocate memory with next power of 2
 * @param vector Vector to manipulate
 * @param size Size to allocate
 */
void Vector_ReserveNearest(Vector *vector, size_t size);
/*
 * @breif Reallocate memory to length of array
 * @param vector Vector to manipulate
 */
void Vector_Reduce(Vector *vector);
/*
 * @breif Clear array and set capacity to 2
 * @param vector Vector to manipulate
 */
void Vector_Clear(Vector *vector);
/*
 * @breif Insert a vector at a certain index
 * @param vector Vector to manipulate
 * @param index Index to insert to
 * @param element Element to insert
 */
void Vector_Insert(Vector *vector, unsigned long long index,
                   const void *element);