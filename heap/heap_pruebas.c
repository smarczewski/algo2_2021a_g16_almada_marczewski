#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

/* Función auxiliar que imita el comportamiento de strcmp pero para enteros.
 */
int comparar_enteros(const void *a, const void *b) {
    if (*(int*)a > *(int*)b) {
        return 1;
    }
    if (*(int*)a < *(int*)b) {
        return -1;
    }
    return 0;
}

static void prueba_crear_heap_vacio()
{
    printf("_____________________________________________________________________________________\n\n");
    printf("INICIO DE PRUEBAS CON HEAP VACIO:\n");

    /* Declaro las variables a utilizar*/
    heap_t* heap = heap_crear(comparar_enteros);

    /* Inicio de pruebas */
    print_test("- Prueba crear heap vacio", heap);
    print_test("- Prueba la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("- Prueba heap_esta_vacio devuelve true", heap_esta_vacio(heap));
    print_test("- Prueba heap_ver_max devuelve NULL", heap_ver_max(heap) == NULL);
    print_test("- Prueba desencolar devuelve NULL", heap_desencolar(heap) == NULL);

    /* Destruyo el heap*/
    heap_destruir(heap, NULL);
    printf("_____________________________________________________________________________________\n\n");
}

static void prueba_heap_algunos_elementos()
{
    printf("INICIO DE PRUEBAS HEAP CON ALGUNOS ELEMENTOS:\n");

    /* Declaro las variables a utilizar*/
    heap_t* heap = heap_crear(comparar_enteros);
    int vec[13] = {8, 10, 2, 11, 5, 4, 9, 7, 0, 6, 1, 3, 12};

    /* Agrego algunos elementos desordenados a la pila */
    for (size_t i = 0; i < 12; i++) {
        int* aux = &vec[i];
        heap_encolar(heap, aux);
    }

    /* Inicio de pruebas */
    print_test("- Prueba heap cargado no esta vacio", heap_esta_vacio(heap) == false);
    print_test("- Prueba la cantidad de elementos es 12", heap_cantidad(heap) == 12);
    print_test("- Prueba el maximo es el elemento mas grande encolado (11)", *(int*)heap_ver_max(heap) == 11);
    print_test("- Prueba desencolar devuelve el elemento mas grande (11)", *(int*)heap_desencolar(heap) == 11);
    print_test("- Prueba la cantidad de elementos es 11", heap_cantidad(heap) == 11);
    print_test("- Prueba el maximo se actualizo despues de desencolar (10)", *(int*)heap_ver_max(heap) == 10);
    print_test("- Prueba encolar un nuevo maximo (12)", heap_encolar(heap, &vec[12]));
    print_test("- Prueba la cantidad de elementos es 12", heap_cantidad(heap) == 12);
    print_test("- Prueba el maximo se actualizo despues de encolar un elemento mas grande (12)", *(int*)heap_ver_max(heap) == 12);
    print_test("- Prueba desencolar devuelve el elemento mas grande (12)", *(int*)heap_desencolar(heap) == 12);
    print_test("- Prueba desencolar devuelve el elemento mas grande (10)", *(int*)heap_desencolar(heap) == 10);
    print_test("- Prueba desencolar devuelve el elemento mas grande (9)", *(int*)heap_desencolar(heap) == 9);
    print_test("- Prueba la cantidad de elementos es 9 despues de desencolar 3 veces", heap_cantidad(heap) == 9);
    print_test("- Prueba volver a encolar un nuevo maximo (12)", heap_encolar(heap, &vec[12]));
    print_test("- Prueba la cantidad de elementos es 10", heap_cantidad(heap) == 10);
    print_test("- Prueba el maximo se actualizo despues de encolar un elemento mas grande (12)", *(int*)heap_ver_max(heap) == 12);

    /* Desencolamos todos los elementos del heap */
    for (size_t i = 0; i < 10; i++) {
        heap_desencolar(heap);
    }

    /* Probamos que el heap quedo vacio luego de desencolar todos los elementos */
    print_test("- Prueba la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("- Prueba heap_esta_vacio devuelve true", heap_esta_vacio(heap));

    /* Destruyo el heap*/
    heap_destruir(heap, NULL);
    printf("_____________________________________________________________________________________\n\n");
}

// Recibe un arreglo de enteros y lo desordena
void randomizar_enteros(int* arreglo, size_t n) {
    srand((unsigned int)time(0)); // Genera una seed para rand en base al tiempo actual

    /* Recorremos el arreglo haciendo swaps aleatorios entre elementos */
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = ((size_t)rand()) % n;
        int aux = arreglo[j];
        arreglo[j] = arreglo[i];
        arreglo[i] = aux;
    }
}

static void prueba_heap_volumen()
{
    printf("INICIO DE PRUEBAS HEAP DE VOLUMEN ESTATICO:\n");

    /* Declaro las variables a utilizar*/
    heap_t* heap = heap_crear(comparar_enteros);
    int vec[1000];

    /* Agrego todos los elementos al arreglo */
    for (int i = 0; i < 1000; i++) {
        vec[i] = i;
    }

    /* Randomizamos el arreglo de enteros */
    randomizar_enteros(vec, 1000);

    /* Agrego todos los elementos del arreglo al heap */
    for (int i = 0; i < 1000; i++) {
        int* aux = &vec[i];
        heap_encolar(heap, aux);
    }

    /* Inicio de pruebas con 1000 elementos */
    print_test("- Prueba heap cargado no esta vacio", heap_esta_vacio(heap) == false);
    print_test("- Prueba la cantidad de elementos es 1000", heap_cantidad(heap) == 1000);
    print_test("- Prueba el maximo es el elemento mas grande encolado (999)", *(int*)heap_ver_max(heap) == 999);

    /* Sacamos todos los elementos del heap */
    bool result = true; // Si el orden de los elementos es correcto devolvemos verdadero
    for (int i = 999; i >= 0; i--) {
        int* desapilado = heap_desencolar(heap);
        if (i != *desapilado) { // Si alguno de los datos no coincide
            result = false;
        }
    }

    /* Verificamos que los elementos desapilados estaban en el orden correcto*/
    print_test("- Prueba se desencolaron todos los elementos y estaban en el orden correcto", result);

    /* Probamos que el heap quedo vacio luego de desencolar todos los elementos */
    print_test("- Prueba la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("- Prueba heap_esta_vacio devuelve true", heap_esta_vacio(heap));

    /* Destruyo el heap*/
    heap_destruir(heap, NULL);
    printf("_____________________________________________________________________________________\n\n");
}

void randomizar_enteros_dinamico(int** arreglo, size_t n) {
    srand((unsigned int)time(0)); // Genera una seed para rand en base al tiempo actual

    /* Recorremos el arreglo haciendo swaps aleatorios entre elementos */
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = ((size_t)rand()) % n;
        int* aux = arreglo[j];
        arreglo[j] = arreglo[i];
        arreglo[i] = aux;
    }
}

static void prueba_heap_volumen_dinamico()
{
    printf("INICIO DE PRUEBAS HEAP DE VOLUMEN DINAMICO:\n");

    /* Declaro las variables a utilizar*/
    heap_t* heap = heap_crear(comparar_enteros);
    int* vec[1000];

    /* Agrego todos los elementos al arreglo en memoria dinamica */
    for (int i = 0; i < 1000; i++) {
        vec[i] = malloc(sizeof(int));
        *(vec[i]) = i;
    }

    /* Randomizamos el arreglo de enteros */
    randomizar_enteros_dinamico(vec, 1000);

    /* Agrego todos los elementos del arreglo al heap */
    for (int i = 0; i < 1000; i++) {
        heap_encolar(heap, vec[i]);
    }

    /* Inicio de pruebas con 1000 elementos */
    print_test("- Prueba heap cargado no esta vacio", heap_esta_vacio(heap) == false);
    print_test("- Prueba la cantidad de elementos es 1000", heap_cantidad(heap) == 1000);
    print_test("- Prueba el maximo es el elemento mas grande encolado (999)", *(int*)heap_ver_max(heap) == 999);

    /* Sacamos todos los elementos del heap */
    bool result = true; // Si el orden de los elementos es correcto devolvemos verdadero
    for (int i = 999; i >= 0; i--) {
        int* desapilado = heap_desencolar(heap);
        if (i != *desapilado) { // Si alguno de los datos no coincide
            result = false;
        }
        free(desapilado);
    }

    /* Verificamos que los elementos desapilados estaban en el orden correcto*/
    print_test("- Prueba se desencolaron todos los elementos y estaban en el orden correcto", result);

    /* Probamos que el heap quedo vacio luego de desencolar todos los elementos */
    print_test("- Prueba la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("- Prueba heap_esta_vacio devuelve true", heap_esta_vacio(heap));

    /* Destruyo el heap con free*/
    heap_destruir(heap, free);
    printf("_____________________________________________________________________________________\n\n");
}

static void prueba_heap_heapify()
{
    printf("INICIO DE PRUEBAS HEAPIFY:\n");

    /* Declaro las variables a utilizar*/
    heap_t* heap = heap_crear(comparar_enteros);
    int vec[7] = {7, 1, 2, 3, 30, 10, 80};
    void* arreglo[7];

    /* Agrego algunos elementos del vector a la cola de prioridad */
    for (size_t i = 0; i < 7; i++) {
        int* aux = &vec[i];
        heap_encolar(heap, aux);
        arreglo[i] = &vec[i];
    }

    /*Convierto el arreglo en un heap*/
    heap_t* arreglo_h = heap_crear_arr(arreglo, 7, comparar_enteros);

    bool ok = true; //Compruebo que los elementos de arreglo_h estén el orden correcto
    for (size_t i = 0; i < 7; i++) {
        if (heap_desencolar(heap) != heap_desencolar(arreglo_h)) {
            ok = false;
        }
    }

    print_test("- Prueba de que los elementos están en el orden correcto", ok);
    heap_destruir(heap,NULL);
    heap_destruir(arreglo_h,NULL);
    printf("_____________________________________________________________________________________\n\n");
}

static void prueba_heap_sort()
{
    printf("INICIO DE PRUEBAS DE HEAP-SORT:\n");

    /* Declaro las variables a utilizar */
    int vec[8] = {4, 2, 7, 10, 4, 3, 8, 5};
    int vec_ord[8] = {2, 3, 4, 4, 5, 7, 8, 10};
    void* arreglo[8];

    /* Agrego punteros a los elementos de vec en arreglo */
    for (size_t i = 0; i < 8; i++) {
        arreglo[i] = &vec[i];
    }

    /*Ordeno los elementos de vec con heap sort */
    heap_sort(arreglo, 8, comparar_enteros);

    bool ok = true;
    for (size_t i = 0; i < 8; i++) { //Comprueba si todos los elementos son los correctos
        if (*((int*) arreglo[i]) != vec_ord[i]) {
            ok = false;
        }
    }

    print_test("- Prueba de que los elementos están en el orden correcto", ok);
    printf("_____________________________________________________________________________________\n\n");
}

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

void pruebas_heap_estudiante()
{
    /* Ejecuta todas las pruebas unitarias. */
    prueba_crear_heap_vacio();
    prueba_heap_algunos_elementos();
    prueba_heap_volumen();
    prueba_heap_volumen_dinamico();
    prueba_heap_heapify();
    prueba_heap_sort();
}

#ifndef CORRECTOR
int main() {
    printf("~~~ PRUEBAS ESTUDIANTE ~~~\n");
    pruebas_heap_estudiante();

    return failure_count() > 0;
}
#endif

