#include "pila.h"
#include "testing.h"
#include <stdio.h>

static void prueba_pila_vacia(void) {
    printf("_____________________________________________________________________________\n\n");
    printf("INICIO DE PRUEBAS CON PILA VACIA\n");

    /* Declaro las variables a utilizar*/
    pila_t *pila = pila_crear();

    /* Inicio de pruebas */
    print_test("- Crear pila vacia", pila != NULL);
    print_test("- Pila creada esta vacia", pila_esta_vacia(pila) == true);
    print_test("- El tope de la pila vacia es NULL", pila_ver_tope(pila) == NULL);
    print_test("- Desapilar en pila vacia es invalido", pila_desapilar(pila) == NULL);

    /* Destruyo la pila*/
    pila_destruir(pila);

    printf("_____________________________________________________________________________\n\n");
}

static void pruebas_pila_algunos_elementos(void) {
    printf("INICIO DE PRUEBAS CON ALGUNOS ELEMENTOS\n");

    /* Declaro las variables a utilizar*/
    pila_t *pila = pila_crear();
    int vec[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    /* Agrego algunos elementos a la pila*/
    for (size_t i = 0; i < 12; i++) {
        int* aux = &vec[i];
        pila_apilar(pila, aux);
    }

    /* Inicio de pruebas */
    print_test("- Pila cargada no esta vacia", pila_esta_vacia(pila) == false);
    print_test("- El tope de la pila es el ultimo elemento apilado (11)", *(int*)pila_ver_tope(pila) == 11);
    print_test("- Desapilar devuelve el ultimo elemento apilado (11)", *(int*)pila_desapilar(pila) == 11);
    print_test("- El tope de la pila se actualizo despues de desapilar (10)", *(int*)pila_ver_tope(pila) == 10);

    /* Sacamos todos los elementos a la pila*/
    for (size_t i = 0; i < 11; i++) {
        pila_desapilar(pila);
    }

    /* Inicio de pruebas */
    print_test("- Pila esta vacia despues de desapilar todos los elementos", pila_esta_vacia(pila) == true);
    print_test("- El tope de la pila vacia es NULL", pila_ver_tope(pila) == NULL);
    print_test("- Desapilar en pila vacia es invalido", pila_desapilar(pila) == NULL);
    print_test("- Se puede apilar NULL", pila_apilar(pila, NULL) == true);
    print_test("- El tope de la pila es NULL", (void*)pila_ver_tope(pila) == NULL);


    /* Destruyo la pila*/
    pila_destruir(pila);

    printf("_____________________________________________________________________________\n\n");
}

static void pruebas_pila_volumen(void) {
    printf("INICIO DE PRUEBAS DE VOLUMEN\n");

    /* Declaro las variables a utilizar*/
    pila_t *pila = pila_crear();
    size_t vec[1000];

    /* Agrego todos los elementos a la pila*/
    for (size_t i = 0; i < 1000; i++) {
        vec[i] = i;
        pila_apilar(pila, &vec[i]);
    }

    /* Inicio de pruebas con 1000 elementos */
    printf("* Pruebas con 1000 elementos: \n");
    print_test("- Pila cargada no esta vacia", pila_esta_vacia(pila) == false);
    print_test("- El tope de la pila es el ultimo elemento apilado (999)", *(size_t*)pila_ver_tope(pila) == 999);
    print_test("- Desapilar devuelve el ultimo elemento apilado (999)", *(size_t*)pila_desapilar(pila) == 999);
    print_test("- El tope de la pila se actualizo despues de desapilar (998)", *(size_t*)pila_ver_tope(pila) == 998);

    /* Sacamos todos los elementos de la pila*/
    bool result = true; // Si el orden de los elementos es correcto devolvemos verdadero
    for (size_t i = 0, j = 998; i < 999; i++, j--) {
        size_t* desapilado = pila_desapilar(pila);
        if (vec[j] != *desapilado) { // Si alguno de los datos no coincide
            result = false;
        }
    }
    /* Verificamos que los elementos desapilados estaban en el orden correcto*/
    print_test("- Se desapilaron los elementos de la pila y estan en el orden correcto", result);

    /* Inicio de pruebas con la pila vacia y el puntero NULL */
    printf("* Pruebas con 0 elementos y cargando el puntero NULL: \n");
    print_test("- Pila esta vacia despues de desapilar todos los elementos", pila_esta_vacia(pila) == true);
    print_test("- El tope de la pila vacia es NULL", pila_ver_tope(pila) == NULL);
    print_test("- Desapilar en pila vacia es invalido", pila_desapilar(pila) == NULL);
    print_test("- Se puede apilar NULL", pila_apilar(pila, NULL) == true);
    print_test("- El tope de la pila es NULL", (void*)pila_ver_tope(pila) == NULL);


    /* Destruyo la pila*/
    pila_destruir(pila);
    printf("_____________________________________________________________________________\n\n");
}


void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    pruebas_pila_algunos_elementos();
    pruebas_pila_volumen();
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
