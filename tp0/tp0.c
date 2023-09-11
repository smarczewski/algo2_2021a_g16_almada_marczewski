#include "tp0.h"

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap(int *x, int *y) {
    int aux = *x;
    *x = *y;
    *y = aux;
}


int maximo(int vector[], int n) {
    if (n <= 0) {
        return -1;
    }
    int max = 0;
    for (int i = 1; i < n; i++) {
        if (vector[i] > vector[max]) {
            max = i;
        }
    }
    return max;
}


int comparar(int vector1[], int n1, int vector2[], int n2) {
    for (int i = 0; i < n1 && i < n2; i++) {
        if (vector1[i] > vector2[i]) {
            return 1;
        } else if (vector1[i] < vector2[i]) {
            return -1;
        }
    }
    if (n1 == n2) {
        return 0;
    } if ( n1 > n2) {
        return 1;
    }
    return -1;
}


void seleccion(int vector[], int n) {
    if (n <= 0) {
        return;
    }
    int idx = n;
    for (int i = 0; i < (n - 1); i++) {
        int max = maximo(vector, idx);
        idx--;
        swap(&vector[max], &vector[idx]);
    }
}
