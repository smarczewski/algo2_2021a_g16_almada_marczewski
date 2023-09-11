#include "lista.h"
#include <stdlib.h>

/* Definición del struct lista y struct nodo.
 */
typedef struct nodo {
    void *dato;
    struct nodo *sig;
} nodo_t;

struct lista {
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
};

/* Definición del iterador externo.
 */

struct lista_iter {
    lista_t* lista;
    nodo_t* actual;
    nodo_t* anterior;
};

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(void) {
    lista_t* ptr_lista = malloc(sizeof(lista_t));

    if (ptr_lista == NULL) {
        return NULL;
    }
    
    ptr_lista->primero = NULL;
    ptr_lista->ultimo = NULL;
    ptr_lista->largo = 0;
    return ptr_lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return lista->largo == 0;
}

nodo_t* crear_nodo(void *dato) {
    nodo_t* ptr_nodo = malloc(sizeof(nodo_t));

    if (ptr_nodo == NULL) {
        return NULL;
    }
    ptr_nodo->dato = dato;
    ptr_nodo->sig = NULL;
    return ptr_nodo;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t* nodo = crear_nodo(dato);
    if (nodo == NULL) {
        return false;
    }
    if (lista_esta_vacia(lista)) {
        lista->ultimo = nodo;
    } else {
        nodo->sig = lista->primero;        
    }
    lista->primero = nodo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t* nodo = crear_nodo(dato);
    if (nodo == NULL) {
        return false;
    }
    if (lista_esta_vacia(lista)) {
        lista->primero = nodo;
    } else {
        lista->ultimo->sig = nodo;
    }
    lista->ultimo = nodo;
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }

    nodo_t* aux = lista->primero;
    lista->primero = lista->primero->sig;
    lista->largo--;

    if (lista_esta_vacia(lista)) {
        lista->ultimo = NULL;
    }

    void* valor = aux->dato;
    free(aux);
    return valor;
}

void *lista_ver_primero(const lista_t *lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
    while (!(lista_esta_vacia(lista))) {
        if (destruir_dato != NULL) {
            destruir_dato(lista->primero->dato);
        }
        lista_borrar_primero(lista);
    }
    free(lista);
}

/* ******************************************************************
 *                 PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    nodo_t* nodo = lista->primero;
    bool seguir = true;
    while (seguir && (nodo != NULL)) {
        void* valor = nodo->dato;
        seguir = visitar(valor, extra);
    	nodo = nodo->sig;
    }
}

/* ******************************************************************
 *                 PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t* ptr_iter = malloc(sizeof(lista_iter_t));

    if (ptr_iter == NULL) {
        return NULL;
    }
    
    ptr_iter->lista = lista;
    ptr_iter->actual = lista->primero;
    ptr_iter->anterior = NULL;
    return ptr_iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) {
        return false;
    }
    iter->anterior = iter->actual;
    iter->actual = iter->actual->sig;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) {
        return NULL;
    }
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    if (iter->actual == iter->lista->primero) { // Si estamos al comienzo
        lista_insertar_primero(iter->lista, dato);
        iter->actual = iter->lista->primero;
        return true;
    }
    if (lista_iter_al_final(iter)) { // Si estamos al final
        lista_insertar_ultimo(iter->lista, dato);
        iter->actual = iter->lista->ultimo;
        return true;
    }
    nodo_t* nodo = crear_nodo(dato);
    if (nodo == NULL) {
        return false;
    }
    nodo->sig = iter->actual;
    iter->actual = nodo;
    iter->anterior->sig = iter->actual;
    iter->lista->largo++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) {
        return NULL;    
    }
    nodo_t* aux = iter->actual;
    if (iter->actual == iter->lista->primero) { // Si estamos al comienzo
        iter->actual = iter->actual->sig;
        void* valor = lista_borrar_primero(iter->lista);
        return valor;
    }
    if (iter->actual == iter->lista->ultimo) { // Si estamos en el ultimo elemento
        iter->anterior->sig = NULL;
        iter->lista->ultimo = iter->anterior;
        iter->actual = NULL;
    } else { // Si estamos en el medio
        iter->actual = iter->actual->sig;
        iter->anterior->sig = iter->actual;
    }
    iter->lista->largo--;
    void* valor = aux->dato;
    free(aux);
    return valor;
}