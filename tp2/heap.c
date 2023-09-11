#include "heap.h"
#include <stdlib.h>

#define CAPACIDAD_INICIAL 10

/* Definición del struct heap.
 */
struct heap{
    void** datos;
    size_t cant;
    size_t tam;
    cmp_func_t cmp;
};

/* *****************************************************************
 *                  FUNCIONES AUXILIARES DEL HEAP
 * *****************************************************************/

/* Función auxiliar que calcula la posicion del padre dado su hijo.
 */
size_t calcular_padre(size_t pos_hijo) {
    return ((pos_hijo - 1) / 2);
}

/* Función auxiliar que calcula la posicion del hijo izquierdo dado su padre.
 */
size_t calcular_hijo_izq(size_t pos_padre) {
    return ((2 * pos_padre) + 1);
}

/* Función auxiliar que calcula la posicion del hijo derecho dado su padre.
 */
size_t calcular_hijo_der(size_t pos_padre) {
    return ((2 * pos_padre) + 2);
}

/* Función auxiliar que redimension el heap de ser necesario.
 */
bool heap_redimensionar(heap_t *heap, size_t tam_nuevo) {
    void **datos_nuevo = realloc(heap->datos, tam_nuevo * sizeof(void*));

    if (tam_nuevo > 0 && datos_nuevo == NULL) {
        return false;
    }

    heap->datos = datos_nuevo;
    heap->tam = tam_nuevo;
    return true;
}

/* Función auxiliar que intercambia dos elementos de
 * un arreglo dadas sus posiciones.
 */
void swap(void** arreglo, size_t pos_1, size_t pos_2) {
    void* aux = arreglo[pos_1];
    arreglo[pos_1] = arreglo[pos_2];
    arreglo[pos_2] = aux;
}

/* Función auxiliar que realiza la operacion upheap recursivamente
 * hasta que un arreglo cumpla con la propiedad de heap.
 */
void upheap(void** arreglo, size_t hijo, cmp_func_t cmp) {
    if (hijo <= 0) return; // Si llegamos al inicio del arreglo
    size_t padre = calcular_padre(hijo);
    if (cmp(arreglo[padre], arreglo[hijo]) < 0) { // Si el padre es menor al hijo
        swap(arreglo, padre, hijo);
        upheap(arreglo, padre, cmp);
    }
}

/* Función auxiliar que devuelve la posicion del maximo
 * entre un elemento padre y sus hijos, si los tiene.
 */
size_t maximo(void** arreglo, size_t cant, size_t padre, size_t h_izq, size_t h_der, cmp_func_t cmp) {
    size_t result = padre;
    if (h_izq <= (cant - 1)) {
        if (cmp(arreglo[result], arreglo[h_izq]) < 0) result = h_izq; // Si el padre es menor al hijo
    }
    if (h_der <= (cant - 1)) {
        if (cmp(arreglo[result], arreglo[h_der]) < 0) result = h_der; // Si el padre/h_izq es menor al hijo_der
    }
    return result;
}

/* Función auxiliar que realiza la operacion downheap recursivamente
 * hasta que un arreglo cumpla con la propiedad de heap.
 */
void downheap(void** arreglo, size_t cant, size_t padre, cmp_func_t cmp) {
    if (padre == (cant - 1)) return;  // Si llegamos al final del arreglo
    size_t h_izq = calcular_hijo_izq(padre);
    size_t h_der = calcular_hijo_der(padre);

    size_t max = maximo(arreglo, cant, padre, h_izq, h_der, cmp); // Obtenemos el mas grande entre el padre y sus hijos
    if (max != padre) {
        swap(arreglo, max, padre);
        downheap(arreglo, cant, max, cmp);
    }
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) { 
    heap_t* h_arreglo = heap_crear_arr(elementos, cant, cmp);

    for (size_t i = 1; i < cant; i++) {
        swap(h_arreglo->datos, 0, cant-i);
        downheap(h_arreglo->datos, cant-i, 0, cmp);
    }

    for (size_t i = 0; i < cant; i++) {
        elementos[i] = h_arreglo->datos[i];
    }
    heap_destruir(h_arreglo, NULL);
}

/* *****************************************************************
 *                      PRIMITIVAS DEL HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp){
    heap_t* ptr_heap = malloc(sizeof(heap_t));

    if (ptr_heap == NULL) {
        return NULL;
    }
    ptr_heap->datos = malloc(sizeof(void*) * CAPACIDAD_INICIAL);

    if (ptr_heap->datos == NULL) {
        return NULL;
    }
    ptr_heap->cant = 0;
    ptr_heap->tam = CAPACIDAD_INICIAL;
    ptr_heap->cmp = cmp;
    return ptr_heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    heap_t* ptr_heap = malloc(sizeof(heap_t));

    if (ptr_heap == NULL) {
        return NULL;
    }

    ptr_heap->datos = malloc(sizeof(void*)*n);
    if (ptr_heap->datos == NULL) {
        free(ptr_heap);
        return NULL;
    }

    for (size_t i = 0; i < n; i++) {
        ptr_heap->datos[i] = arreglo[i];
    }

    for (size_t i = 0; i < n/2 ; i++) {
        downheap(ptr_heap->datos, n, n/2-1-i, cmp);
    }

    ptr_heap->cant = n;
    ptr_heap->tam = n;
    ptr_heap->cmp = cmp;
    return ptr_heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    if (destruir_elemento != NULL) {
        for (size_t i = 0; i < heap->cant; i++) {
            destruir_elemento(heap->datos[i]);
        }
    }
    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap) {
    return heap->cant == 0;
}

bool heap_encolar(heap_t *heap, void *elem) {
    if (heap->cant == heap->tam) { // Redimensionamos de ser necesario
        size_t tam_nuevo = heap->tam * 2;
        if (!(heap_redimensionar(heap, tam_nuevo))) {
            return false;
        }
    }
    heap->datos[heap->cant] = elem; // Insertamos el nuevo elemento al final
    heap->cant++;
    upheap(heap->datos, heap->cant - 1, heap->cmp); // Aplicamos upheap hasta que se cumpla propiedad de heap
    return true;
}

void *heap_ver_max(const heap_t *heap) {
    if (heap_esta_vacio(heap)) {
        return NULL;
    }
    return heap->datos[0];
}

void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) {
        return NULL;
    }
    void* elem = heap->datos[0]; // Guardamos el maximo a desencolar

    if (heap_cantidad(heap) >= 2) { // Si hay al menos dos elemenos hacemos swap
        swap(heap->datos, 0, heap->cant - 1);
        downheap(heap->datos, heap->cant - 1, 0, heap->cmp);
    }

    heap->cant--;

    size_t tam_nuevo = heap->tam / 2;
    if ((heap->cant * 4 <= heap->tam) && (tam_nuevo > CAPACIDAD_INICIAL)) {
        heap_redimensionar(heap, tam_nuevo);
    }

    return elem;
}
