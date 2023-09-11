#include "pila.h"

#include <stdlib.h>

#define CAPACIDAD_INICIAL 10

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void) {
    pila_t* ptr_pila = malloc(sizeof(pila_t));

    if (ptr_pila == NULL) {
        return NULL;
    }
    ptr_pila->datos = malloc(sizeof(void*) * CAPACIDAD_INICIAL);

    if (ptr_pila->datos == NULL) {
        return NULL;
    }
    ptr_pila->cantidad = 0;
    ptr_pila->capacidad = CAPACIDAD_INICIAL;
    return ptr_pila;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
    return pila->cantidad == 0;
}

bool pila_redimensionar(pila_t *pila, size_t tam_nuevo) {
    void **datos_nuevo = realloc(pila->datos, tam_nuevo * sizeof(void*));

    if (tam_nuevo > 0 && datos_nuevo == NULL) {
        return false;
    }

    pila->datos = datos_nuevo;
    pila->capacidad = tam_nuevo;
    return true;
}

bool pila_apilar(pila_t *pila, void *valor) {
    if (pila->cantidad == pila->capacidad) {
        size_t cap_nueva = pila->capacidad * 2;
        if (!(pila_redimensionar(pila, cap_nueva))) {
            return false;
        }
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    if (pila_esta_vacia(pila)) {
        return NULL;
    }
    return pila->datos[(pila->cantidad) - 1];
}

void *pila_desapilar(pila_t *pila) {
    if (pila_esta_vacia(pila)) {
        return NULL;
    }
    void* valor = pila->datos[pila->cantidad - 1];
    pila->cantidad--;

    size_t cap_nueva = pila->capacidad / 2;
    if ((pila->cantidad * 4 <= pila->capacidad) && (cap_nueva > CAPACIDAD_INICIAL)) {
        pila_redimensionar(pila, cap_nueva);
    }
    return valor;
}

