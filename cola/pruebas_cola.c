#include "cola.h"
#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>

/* La funcion wrapper enmascara el comportamiento de pila_destruir
para que se pueda pasar por parametro a cola_destruir.  */
void pila_destruir_wrapper(void* pila) {
    pila_destruir(pila);
}

static void prueba_cola_vacia(void) {
    printf("______________________________________________________________________________\n\n");
    printf("INICIO DE PRUEBAS CON COLA VACIA\n");

    /* Declaro las variables a utilizar*/
    cola_t *cola = cola_crear();

    /* Inicio de pruebas */
    print_test("- Crear cola vacia", cola != NULL);
    print_test("- Cola creada esta vacia", cola_esta_vacia(cola) == true);
    print_test("- El primero de la cola vacia es NULL", cola_ver_primero(cola) == NULL);
    print_test("- Desencolar en cola vacia es invalido", cola_desencolar(cola) == NULL);

    /* Destruyo la cola con funcion NULL*/
    cola_destruir(cola, NULL);
    print_test("- Se destruyo correctamente la lista vacia sin funcion de destruccion", true);

    /* Creo y destruyo una nueva lista con funcion no NULL*/
    cola = cola_crear();
    cola_destruir(cola, pila_destruir_wrapper);
    print_test("- Se destruyo correctamente la cola vacia con funcion de destruccion", true);

    printf("______________________________________________________________________________\n\n");
}

static void pruebas_cola_algunos_elementos(void) {
    printf("INICIO DE PRUEBAS CON ALGUNOS ELEMENTOS SIN FUNCION DE DESTRUCCION\n");

    /* Declaro las variables a utilizar*/
    cola_t *cola = cola_crear();
    int vec[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    /* Agrego algunos elementos a la cola*/
    for (size_t i = 0; i < 12; i++) {
        int* aux = &vec[i];
        cola_encolar(cola, aux);
    }

    /* Inicio de pruebas */
    print_test("- Cola cargada no esta vacia", cola_esta_vacia(cola) == false);
    print_test("- El primero de la cola es el primer elemento acolado (0)", *(int*)cola_ver_primero(cola) == 0);
    print_test("- Desencolar devuelve el primer elemento encolado (0)", *(int*)cola_desencolar(cola) == 0);
    print_test("- El primero de la cola se actualizo despues de desencolar (1)", *(int*)cola_ver_primero(cola) == 1);

    /* Sacamos todos los elementos a la cola*/
    for (size_t i = 0; i < 11; i++) {
        cola_desencolar(cola);
    }

    /* Inicio de pruebas */
    print_test("- Cola esta vacia despues de desencolar todos los elementos", cola_esta_vacia(cola) == true);
    print_test("- El primero de la cola vacia es NULL", cola_ver_primero(cola) == NULL);
    print_test("- Desencolar en cola vacia es invalido", cola_desencolar(cola) == NULL);
    print_test("- Se puede encolar NULL", cola_encolar(cola, NULL) == true);
    print_test("- El primero de la cola es NULL", (void*)cola_ver_primero(cola) == NULL);


    /* Destruyo la cola*/
    cola_destruir(cola, NULL);

    printf("______________________________________________________________________________\n\n");
}

static void pruebas_cola_volumen(void) {
    printf("INICIO DE PRUEBAS DE VOLUMEN SIN FUNCION DE DESTRUCCION\n");

    /* Declaro las variables a utilizar*/
    cola_t *cola = cola_crear();
    size_t vec[1000];

    /* Agrego todos los elementos a la lista*/
    for (size_t i = 0; i < 1000; i++) {
        vec[i] = i;
        cola_encolar(cola, &vec[i]);
    }

    /* Inicio de pruebas con 1000 elementos */
    printf("* Pruebas con 1000 elementos: \n");
    print_test("- La cola cargada no esta vacia", cola_esta_vacia(cola) == false);
    print_test("- El primero de la cola es el primer elemento encolado (0)", *(size_t*)cola_ver_primero(cola) == 0);
    print_test("- Desencolar devuelve el primer elemento encolado (0)", *(size_t*)cola_desencolar(cola) == 0);
    print_test("- El primero de la cola se actualizo despues de desencolar (1)", *(size_t*)cola_ver_primero(cola) == 1);

    /* Sacamos todos los elementos de la cola*/
    bool result = true; // Si el orden de los elementos es correcto devolvemos verdadero
    for (size_t i = 1; i < 1000; i++) {
        size_t* desencolado = cola_desencolar(cola);
        if (vec[i] != *desencolado) { // Si alguno de los datos no coincide
            result = false;
        }
    }
    /* Verificamos que los elementos desencolados estaban en el orden correcto*/
    print_test("- Se desencolaron los elementos de la cola y estan en el orden correcto", result);

    /* Inicio de pruebas con la cola vacia y el puntero NULL */
    printf("* Pruebas con 0 elementos y cargando el puntero NULL: \n");
    print_test("- Cola esta vacia despues de desencolar todos los elementos", cola_esta_vacia(cola) == true);
    print_test("- El primero de la cola vacia es NULL", cola_ver_primero(cola) == NULL);
    print_test("- Desencolar en cola vacia es invalido", cola_desencolar(cola) == NULL);
    print_test("- Se puede encolar NULL", cola_encolar(cola, NULL) == true);
    print_test("- El primero de la cola es NULL", (void*)cola_ver_primero(cola) == NULL);


    /* Destruyo la cola*/
    cola_destruir(cola, NULL);
    printf("______________________________________________________________________________\n\n");
}

static void pruebas_cola_con_pilas(void) {
    printf("INICIO DE PRUEBAS CON FUNCION DE DESTRUCCION DE LOS ELEMENTOS\n");

    /* Declaro las variables a utilizar*/
    cola_t *cola = cola_crear();
    int *arreglo[1000];

    /* Agrego ints a la cola*/
    for (size_t i = 0; i < 1000; i++) {
        int *aux = malloc(sizeof(int));
        if (aux != NULL) {
            arreglo[i] = aux;
        }
        cola_encolar(cola, arreglo[i]);
    }

    /* Inicio de pruebas con free() */
    print_test("- Se creo la cola y se cargo con ints usando malloc", cola_esta_vacia(cola) == false);

    /* Destruyo la cola pasando free() por parametro*/
    cola_destruir(cola, free);

    print_test("- Se destruyo la cola y todos los ints encolados usando free", true);


    /* Declaro las variables a utilizar*/
    cola_t *cola_con_pilas = cola_crear();
    pila_t *arreglo_de_pilas[1000];

    /* Agrego mil pilas a la cola*/
    for (size_t i = 0; i < 1000; i++) {
        arreglo_de_pilas[i] = pila_crear();
        cola_encolar(cola_con_pilas, arreglo_de_pilas[i]);
    }

    /* Inicio de pruebas con pila_destruir */
    print_test("- Se creo la cola y se cargo con pilas", cola_esta_vacia(cola_con_pilas) == false);

    /* Destruyo la cola pasando pila_destruir por parametro*/
    cola_destruir(cola_con_pilas, pila_destruir_wrapper);

    print_test("- Se destruyo la cola y todas las pilas encoladas con pila_destruir", true);

    printf("______________________________________________________________________________\n\n");
}


void pruebas_cola_estudiante() {
    prueba_cola_vacia();
    pruebas_cola_algunos_elementos();
    pruebas_cola_volumen();
    pruebas_cola_con_pilas();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
