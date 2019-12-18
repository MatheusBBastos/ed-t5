#include "binary_heap.h"

typedef struct binary_heap_h {
    Value *nodes;
    int length;
    int (*compare)(const void *a, const void *b);
} *BinHeapImpl;

BinaryHeap BinHeap_Create(int size, int (*compare)(const void *a, const void *b)) {
    BinHeapImpl heap = malloc(sizeof(struct binary_heap_h));
    heap->nodes = malloc(size * sizeof(Value));
    heap->nodes[0] = NULL;
    heap->compare = compare;
    heap->length = 0;
}

BinaryHeap BinHeap_Insert(BinaryHeap heapVoid, Value element) {
    BinHeapImpl heap = (BinHeapImpl) heapVoid;
    int i = heap->length;
    heap->nodes[i] = element;

    Value temp;
    int parent = (i-1)/2;

    while (i != 0 && heap->compare(element, heap->nodes[parent]) < 0) {
        temp = heap->nodes[parent];
        heap->nodes[parent] = element;
        heap->nodes[i] = temp;
        i = parent;
        parent = (i-1)/2;
    }

    heap->length++;
}

static int _pickChild(BinHeapImpl heap, int i) {
    if (i * 2 + 2 >= heap->length || heap->compare(heap->nodes[i*2 + 1], heap->nodes[i*2 + 2]) < 0) {
        return i * 2 + 1;
    } else {
        return i * 2 + 2;
    }
}

Value BinHeap_Extract(BinaryHeap heapVoid) {
    BinHeapImpl heap = (BinHeapImpl) heapVoid;

    if (heap->length == 0)
        return NULL;

    Value value = heap->nodes[0];
    Value temp;

    heap->nodes[0] = heap->nodes[--heap->length];
    int i = 0, child;

    while (i * 2 + 1 < heap->length) {
        child = _pickChild(heap, i);
        if (heap->compare(heap->nodes[i], heap->nodes[child]) > 0) {
            temp = heap->nodes[i];
            heap->nodes[i] = heap->nodes[child];
            heap->nodes[child] = temp;
        }
        i = child;
    }

    return value;
}

bool BinHeap_IsEmpty(BinaryHeap heap) {
    return ((BinHeapImpl) heap)->length == 0;
}

void BinHeap_Destroy(BinaryHeap heapVoid) {
    BinHeapImpl heap = (BinHeapImpl) heapVoid;

    free(heap->nodes);
    free(heap);
}
