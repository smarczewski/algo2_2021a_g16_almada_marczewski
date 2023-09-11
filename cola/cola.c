#include "cola.h"

#include <stdlib.h>

/* Definición del struct cola y struct nodo.
 */
typedef struct nodo {
    void *dato;
    struct nodo *sig;
} nodo_t;

struct cola {
    nodo_t* primero;
    nodo_t* ultimo;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t *cola_crear(void) {
    cola_t* ptr_cola = malloc(sizeof(cola_t));

    if (ptr_cola == NULL) {
        return NULL;
    }
    
    ptr_cola->primero = NULL;
    ptr_cola->ultimo = NULL;
    return ptr_cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {
    while (!(cola_esta_vacia(cola))) {
        if (destruir_dato != NULL) {
            destruir_dato(cola->primero->dato);
        }
        cola_desencolar(cola);
    }
    free(cola);
}

bool cola_esta_vacia(const cola_t *cola) {
    if (cola->primero != NULL) {
        return false;
    }
    return true;
}

nodo_t* crear_nodo(void *valor) {
    nodo_t* ptr_nodo = malloc(sizeof(nodo_t));
    
    if (ptr_nodo == NULL) {
        return NULL;
    }
    ptr_nodo->dato = valor;
    ptr_nodo->sig = NULL;
    return ptr_nodo;
}

bool cola_encolar(cola_t *cola, void *valor) {
    nodo_t* nodo = crear_nodo(valor);
    if (nodo == NULL) {
        return false;
    }
    if (cola_esta_vacia(cola)) {
        cola->primero = nodo;
        cola->ultimo = nodo;
        return true;
    }
    cola->ultimo->sig = nodo;
    cola->ultimo = nodo;
    return true;
}

void *cola_ver_primero(const cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        return NULL;
    }
    return cola->primero->dato;
}

void *cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        return NULL;
    }

    nodo_t* aux = cola->primero;
    cola->primero = cola->primero->sig;

    if (cola_esta_vacia(cola)) {
        cola->ultimo = NULL;
    }

    void* valor = aux->dato;
    free(aux);
    return valor;
}