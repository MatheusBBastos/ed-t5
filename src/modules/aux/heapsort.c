#include "heapsort.h"

void heapsort(Item *a, int n, int ate, int (*cmp)(const void*, const void*)) {
    int n1 = n/2;
    int i = n / 2, pai, filho;
    Item t;
    int cont = 0;
    while (true) {
        if (i > 0) {
            i--;
            t = a[i];
        } else {
            n--;
            if (n == 0) return;
            t = a[n];
            a[n] = a[0];
        }
        pai = i;
        filho = i * 2 + 1;
        while (filho < n) {
            if ((filho + 1 < n)  &&  (cmp(a[filho + 1], a[filho])) > 0)
                filho++;
            if (cmp(a[filho], t) > 0) {
                Item temp = a[pai];
                a[pai] = a[filho];
                a[filho] = temp;
                pai = filho;
                filho = pai * 2 + 1;
            } else {
                break;
            }
        }
        a[pai] = t;
        cont++;
        if (cont >= ate + n1) return;
    }
}