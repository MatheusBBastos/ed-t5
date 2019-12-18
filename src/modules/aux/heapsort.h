#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <stdbool.h>

typedef void *Item;

// Ordena o vetor 'a' de 'n' elementos apenas até que 'ate' elementos
// estejam ordenados no fim do vetor, comparando os elementos com a função 'cmp'
void heapsort(Item *a, int n, int ate, int (*cmp)(const void*, const void*));

#endif