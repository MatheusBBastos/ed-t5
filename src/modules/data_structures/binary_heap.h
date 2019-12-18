#ifndef BINARYHEAP_H
#define BINARYHEAP_H

#include <stdlib.h>
#include <stdbool.h>

typedef void *BinaryHeap;
typedef void *Value;

BinaryHeap BinHeap_Create(int size, int (*compare)(const void *a, const void *b));

BinaryHeap BinHeap_Insert(BinaryHeap heap, Value element);

Value BinHeap_Extract(BinaryHeap heap);

bool BinHeap_IsEmpty(BinaryHeap heap);

void BinHeap_Destroy(BinaryHeap heap);

#endif