#include "lista.h"
#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>

/* La funcion wrapper enmascara el comportamiento de pila_destruir
para que se pueda pasar por parametro a lista_destruir.  */
void pila_destruir_wrapper(void* pila) {
    pila_destruir(pila);
}

/* Funcion "visitar" para probar el iterador interno.  */
bool comparar_un_item(void *elemento, void *extra) {
    int *contador = extra;
    if (*contador != *(int*)elemento) { // Si alguno de los datos no coincide
        return false;
    }
    ++(*contador);
    return true;  // seguir iterando
}

static void prueba_lista_vacia(void) {
    printf("________________________________________________________________________________\n");
    printf("INICIO DE PRUEBAS CON LISTA VACIA Y DESTRUCCION CON FUNCION NULL/NO NULL \n");

    /* Declaro las variables a utilizar*/
    lista_t *lista = lista_crear();

    /* Inicio de pruebas */
    print_test("- Crear lista vacia", lista != NULL);
    print_test("- lista creada esta vacia", lista_esta_vacia(lista) == true);
    print_test("- El primero de la lista vacia es NULL", lista_ver_primero(lista) == NULL);
    print_test("- Desenlistar en lista vacia es invalido", lista_borrar_primero(lista) == NULL);

    /* Destruyo la lista con funcion NULL*/
    lista_destruir(lista, NULL);
    print_test("- Se destruyo correctamente la lista vacia sin funcion de destruccion", true);

    /* Creo y destruyo una nueva lista con funcion no NULL*/
    lista = lista_crear();
    lista_destruir(lista, pila_destruir_wrapper);
    print_test("- Se destruyo correctamente la lista con vacia funcion de destruccion", true);


    printf("________________________________________________________________________________\n");
}

static void pruebas_lista_algunos_elementos(void) {
    printf("INICIO DE PRUEBAS CON ALGUNOS ELEMENTOS SIN FUNCION DE DESTRUCCION\n");

    /* Declaro las variables a utilizar*/
    lista_t *lista = lista_crear();
    int vec[13] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 99};

    /* Agrego algunos elementos a la lista*/
    for (size_t i = 0; i < 12; i++) {
        int* aux = &vec[i];
        lista_insertar_ultimo(lista, aux);
    }

    /* Agrego un elemento a la primera posicion en la lista*/
    int* aux = &vec[12];
    lista_insertar_primero(lista, aux);

    /* Inicio de pruebas */
    print_test("- La lista cargada no esta vacia", lista_esta_vacia(lista) == false);
    print_test("- El primero de la lista es el agregado con insertar_primero (99))", *(int*)lista_ver_primero(lista) == 99);
    print_test("- Borrar primero devuelve el primer elemento (99)", *(int*)lista_borrar_primero(lista) == 99);
    print_test("- El primero de la lista se actualizo despues de borrar el primero (0)", *(int*)lista_ver_primero(lista) == 0);

    /* Sacamos todos los elementos a la lista*/
    for (size_t i = 0; i < 12; i++) {
        lista_borrar_primero(lista);
    }

    /* Inicio de pruebas */
    print_test("- La lista esta vacia despues de borrar todos los elementos", lista_esta_vacia(lista) == true);
    print_test("- El primero de la lista vacia es NULL", lista_ver_primero(lista) == NULL);
    print_test("- Borrar en lista vacia es invalido", lista_borrar_primero(lista) == NULL);
    print_test("- Se puede agregar NULL a la lista", lista_insertar_ultimo(lista, NULL) == true);
    print_test("- El primero de la lista es NULL", (void*)lista_ver_primero(lista) == NULL);


    /* Destruyo la lista*/
    lista_destruir(lista, NULL);
    print_test("- Se destruyo correctamente la lista con algunos elementos", true);

    printf("________________________________________________________________________________\n");
}

static void pruebas_lista_volumen_null(void) {
    printf("INICIO DE PRUEBAS DE VOLUMEN SIN FUNCION DE DESTRUCCION\n");

    /* Declaro las variables a utilizar*/
    lista_t *lista = lista_crear();
    size_t arreglo[1000];

    /* Agrego todos los elementos a la lista*/
    for (size_t i = 0; i < 1000; i++) {
        arreglo[i] = i;
        lista_insertar_ultimo(lista, &arreglo[i]);
    }

    /* Inicio de pruebas con 1000 elementos */
    printf("* Pruebas con 1000 elementos: \n");
    print_test("- La lista cargada no esta vacia", lista_esta_vacia(lista) == false);
    print_test("- El primero de la lista es el primer elemento insertado (0)", *(size_t*)lista_ver_primero(lista) == 0);
    print_test("- El ultimo de la lista es el ultimo elemento insertado (999)", *(size_t*)lista_ver_ultimo(lista) == 999);

    /* Sacamos todos los elementos de la lista*/
    bool result = true; // Si el orden de los elementos es correcto devolvemos verdadero
    for (size_t i = 0; i < 1000; i++) {
        size_t* borrado = lista_borrar_primero(lista);
        if (arreglo[i] != *borrado) { // Si alguno de los datos no coincide
            result = false;
        }
    }

    /* Inicio de pruebas con la lista vacia y el puntero NULL */
    printf("* Pruebas despues de borrar elementos hasta que la lista este vacia: \n");
    print_test("- Se borraron los elementos de la lista y estan en el orden correcto", result);
    print_test("- lista esta vacia despues de borrar todos los elementos", lista_esta_vacia(lista) == true);
    print_test("- El primero de la lista vacia es NULL", lista_ver_primero(lista) == NULL);
    print_test("- El ultimo de la lista vacia es NULL", lista_ver_ultimo(lista) == NULL);
    print_test("- Borrar en lista vacia es invalido", lista_borrar_primero(lista) == NULL);

    /* Agrego nuevamente todos los elementos a la lista*/
    for (size_t i = 0; i < 1000; i++) {
        lista_insertar_ultimo(lista, &arreglo[i]);
    }

    /* Destruimos la lista no vacia con funcion de destruccion NULL*/
    printf("* Prueba destruir la lista no vacia con funcion de destruccion NULL: \n");
    lista_destruir(lista, NULL);
    print_test("- Se destruyo correctamente la lista no vacia sin funcion de destruccion", true);
    printf("________________________________________________________________________________\n");
}

static void pruebas_lista_volumen_nonull(void) {
    printf("INICIO DE PRUEBAS DE VOLUMEN CON FUNCION DE DESTRUCCION\n");

    /* Declaro las variables a utilizar*/
    lista_t *lista_con_pilas = lista_crear();
    pila_t *arreglo_de_pilas[1000];

    /* Agrego algunas pilas a la lista*/
    for (size_t i = 0; i < 1000; i++) {
        arreglo_de_pilas[i] = pila_crear();
        lista_insertar_ultimo(lista_con_pilas, arreglo_de_pilas[i]);
    }

    /* Inicio de pruebas con 1000 pilas */
    printf("* Pruebas con 1000 pilas: \n");
    print_test("- La lista cargada no esta vacia", lista_esta_vacia(lista_con_pilas) == false);
    print_test("- El primero de la lista es el primer elemento insertado (0)", (pila_t*)lista_ver_primero(lista_con_pilas) == arreglo_de_pilas[0]);
    print_test("- El ultimo de la lista es el ultimo elemento insertado (999)", (pila_t*)lista_ver_ultimo(lista_con_pilas) == arreglo_de_pilas[999]);

    /* Sacamos todos los elementos de la lista*/
    bool result = true; // Si el orden de los elementos es correcto devolvemos verdadero
    for (size_t i = 0; i < 1000; i++) {
        pila_t *borrado = lista_borrar_primero(lista_con_pilas);
        if (arreglo_de_pilas[i] != borrado) { // Si alguno de los datos no coincide
            result = false;
        }
    }

    /* Inicio de pruebas con la lista vacia y el puntero NULL */
    printf("* Pruebas despues de borrar elementos hasta que la lista este vacia: \n");
    print_test("- Se borraron los elementos de la lista y estan en el orden correcto", result);
    print_test("- lista esta vacia despues de borra todos los elementos", lista_esta_vacia(lista_con_pilas) == true);
    print_test("- El primero de la lista vacia es NULL", lista_ver_primero(lista_con_pilas) == NULL);
    print_test("- El ultimo de la lista vacia es NULL", lista_ver_ultimo(lista_con_pilas) == NULL);
    print_test("- Borrar en lista vacia es invalido", lista_borrar_primero(lista_con_pilas) == NULL);

    /* Agrego nuevamente todos los elementos a la lista*/
    for (size_t i = 0; i < 1000; i++) {
        lista_insertar_ultimo(lista_con_pilas, arreglo_de_pilas[i]);
    }

    /* Destruimos la lista no vacia con funcion de destruccion NULL*/
    printf("* Prueba destruir la lista no vacia con funcion de destruccion no NULL: \n");
    lista_destruir(lista_con_pilas, pila_destruir_wrapper);
    print_test("- Se destruyo correctamente la lista no vacia con funcion de destruccion", true);
    printf("________________________________________________________________________________\n");
}

static void pruebas_iterador_externo(void) {
    printf("INICIO DE PRUEBAS ITERADOR EXTERNO\n");

    /* Declaro las variables a utilizar*/
    lista_t *lista = lista_crear();
    size_t arreglo[1000];

    /* Agrego algunos elementos a la lista*/
    for (size_t i = 0; i < 1000; i++) {
        arreglo[i] = i;
        lista_insertar_ultimo(lista, &arreglo[i]);
    }

    /* Creo el iterador externo*/
    lista_iter_t* iter = lista_iter_crear(lista);
    size_t i = 0;
    bool result = true; // Si el orden de los elementos es correcto devolvemos verdadero
    while ( !lista_iter_al_final(iter) ) {
        void* dato = lista_iter_ver_actual(iter);
        if (arreglo[i] != *(size_t*)dato) { // Si alguno de los datos no coincide
            result = false;
        }
        i++;
        lista_iter_avanzar(iter);
    }
    print_test("- Se itero la lista original y todos los elementos estan en orden", result);
    lista_iter_destruir(iter);

    /* Reiniciamos el iterador externo e insertamos un elemento*/
    iter = lista_iter_crear(lista);
    size_t primero = 999;
    lista_iter_insertar(iter, &primero);

    /* Iteramos sobre la lista*/
    while ( !lista_iter_al_final(iter) ) {
        lista_iter_avanzar(iter);
    }

    /* Insertamos un elemento al final de la iteracion*/
    size_t ultimo = 111;
    lista_iter_insertar(iter, &ultimo);

    /* Pruebas insertar al principio y al final*/
    print_test("- Insertar cuando se crea el iterador inserta al principio", *(size_t*)lista_ver_primero(lista) == primero);
    print_test("- Insertar cuando el iterador esta al final inserta al final", *(size_t*)lista_ver_ultimo(lista) == ultimo);
    lista_iter_destruir(iter);

    /* Reiniciamos el iterador externo y borramos un elemento*/
    iter = lista_iter_crear(lista);
    lista_iter_borrar(iter);

    /* Iteramos sobre la lista hasta la ultima posicion y borramos el ultimo elemento*/
    for (size_t a = 0; a < 1000; a++) {
        lista_iter_avanzar(iter);
    }
    lista_iter_borrar(iter);

    /* Pruebas insertar al principio y al final*/
    print_test("- Borrar cuando se crea el iterador cambia el primero de la lista", *(size_t*)lista_ver_primero(lista) != primero);
    print_test("- Borrar cuando el iterador esta al final cambia el ultimo de la lista", *(size_t*)lista_ver_ultimo(lista) != ultimo);
    lista_iter_destruir(iter);

    lista_destruir(lista, NULL);   
    printf("________________________________________________________________________________\n");
}

static void pruebas_iterador_interno(void) {
    printf("INICIO DE PRUEBAS ITERADOR INTERNO\n");

    /* Declaro las variables a utilizar*/
    lista_t *lista = lista_crear();
    size_t arreglo[1000];

    /* Agrego algunos elementos a la lista*/
    for (size_t i = 0; i < 1000; i++) {
        arreglo[i] = i;
        lista_insertar_ultimo(lista, &arreglo[i]);
    }

    /* Iteramos sobre la lista comparando los valores que agregamos, sin condicion de corte*/
    int numero = 0;
    lista_iterar(lista, comparar_un_item, &numero);

    /* Iteramos sobre la lista comparando los valores que agregamos, con condicion de corte
       el elemento que insertamos no va a coincidir, haciendo que visitar de false y la iteracion corte al comienzo*/
    size_t corte = 123; // Insertamos un numero que va a devolver falso al comparar
    lista_insertar_primero(lista, &corte);
    numero = 0; // Reiniciamos el contador
    lista_iterar(lista, comparar_un_item, &numero);

    lista_destruir(lista, NULL);
    print_test("- Se destruyo la lista y todos los elementos cumplen las condiciones", numero == 0);
    printf("________________________________________________________________________________\n");
}

void pruebas_lista_estudiante() {
    prueba_lista_vacia();
    pruebas_lista_algunos_elementos();
    pruebas_lista_volumen_null();
    pruebas_lista_volumen_nonull();
    pruebas_iterador_externo();
    pruebas_iterador_interno();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif