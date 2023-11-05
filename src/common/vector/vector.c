#include "vector.h"
#include <string.h>
// https://graphics.stanford.edu/%7Eseander/bithacks.html#RoundUpPowerOf2
unsigned int NextPowerOf2(unsigned int value) {
  ++value;
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  return ++value;
}
Vector Vector_Create(size_t elementSize) {
  Vector v;
  v.pData = malloc(elementSize * 2);
  v.length = 0;
  v.capacity = 2;
  v.elementSize = elementSize;
  return v;
}
Vector Vector_Copy(Vector *vector) {
  Vector v = Vector_Create(vector->elementSize);
  Vector_Reserve(&v, vector->capacity);
  v.length = vector->length;
  memcpy(v.pData, vector->pData, vector->length * vector->length);
  return v;
}
void *Vector_At(Vector *vector, unsigned long long index) {
  return ((char *)vector->pData) + vector->elementSize * index;
}
void *Vector_PushBack(Vector *vector, void *element) {
  if (vector->length >= vector->capacity) {
    Vector_Reserve(vector, vector->capacity * 2);
  }
  memcpy((char *)vector->pData + vector->length * vector->elementSize, element,
         vector->elementSize);
  return (char *)vector->pData + (vector->length++) * vector->elementSize;
}
void Vector_Pop(Vector *vector) {
  --vector->length;
  if (vector->length < vector->capacity / 2) {
    vector->capacity /= 2;
    vector->pData =
        realloc(vector->pData, vector->capacity * vector->elementSize);
  }
}
void Vector_Dispose(Vector *vector) { free(vector->pData); }
void Vector_RemoveAt(Vector *vector, unsigned long long index, int bOrdered) {
  if (index + 1 == vector->length) {
    Vector_Pop(vector);
    return;
  }
  if (!bOrdered)
    memmove(Vector_At(vector, index), Vector_At(vector, vector->length - 1),
            vector->elementSize);
  else
    memmove(Vector_At(vector, index), Vector_At(vector, index + 1),
            vector->elementSize * (vector->length - index - 1));
  --vector->length;
}
void Vector_Reserve(Vector *vector, size_t size) {
  void *newData = realloc(vector->pData, size * vector->elementSize);
  if (newData == NULL) {
    newData = malloc(size * vector->elementSize);
    memcpy(newData, vector->pData, vector->length * vector->elementSize);
    free(vector->pData);
  }
  vector->pData = newData;
  vector->capacity = size;
}
void Vector_ReserveNearest(Vector *vector, size_t size) {
  size = (size_t)NextPowerOf2(size);
  Vector_Reserve(vector, size);
}
void Vector_Reduce(Vector *vector) { Vector_Reserve(vector, vector->length); }
void Vector_Clear(Vector *vector) {
  vector->length = 0;
  Vector_Reserve(vector, 2);
}
void Vector_Insert(Vector *vector, unsigned long long index, void *element) {
  if (vector->length == index) {
    Vector_PushBack(vector, element);
    return;
  }
  Vector_PushBack(vector, element);
  memmove(Vector_At(vector, index + 1), Vector_At(vector, index),
          vector->elementSize * (vector->length - index));
  memcpy(Vector_At(vector, index), element, vector->elementSize);
}
