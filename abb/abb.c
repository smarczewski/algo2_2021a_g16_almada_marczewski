#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "abb.h"
#include "pila.h"

/* Definición del struct abb y nodo_abb.
 */

typedef struct nodo_abb {
    struct nodo_abb* izq;
    struct nodo_abb* der;
    char* clave;
    void* dato;
} nodo_abb_t;

typedef struct abb {
    nodo_abb_t* raiz;
    size_t cant;
    abb_destruir_dato_t destruir_dato;
    abb_comparar_clave_t comparar;
} abb_t;

/* Definición del iterador externo.
 */
struct abb_iter {
    const abb_t* abb;
	pila_t* pila;
};

/*******************************************************************
 *                FUNCIONES AUXILIARES DEL HASH
 ******************************************************************/

nodo_abb_t* abb_busqueda(nodo_abb_t* nodo, abb_comparar_clave_t comparar, const char* clave, nodo_abb_t*** padre) {
	if (nodo == NULL) return nodo;

	int cmp = comparar(nodo->clave, clave);
	if (cmp == 0) { // Si es la clave que buscamos
		return nodo;
	}

	if (cmp > 0) { // La clave que buscamos es mas chica, vamos para la izquierda
		if (padre != NULL) *padre = &nodo->izq;
		return (abb_busqueda(nodo->izq, comparar, clave, padre));
	} else { // La clave que buscamos es mas grande, vamos para la derecha
		if (padre != NULL) *padre = &nodo->der;
		return (abb_busqueda(nodo->der,comparar, clave, padre));
	}
}

// Devuelve el maximo de un arbol o subarbol, dada su raiz
nodo_abb_t** abb_max(nodo_abb_t** nodo) {
	if ((*nodo)->der == NULL) return nodo;
	return abb_max(&(*nodo)->der);
}

size_t cant_hijos(nodo_abb_t* nodo, nodo_abb_t*** ref_aux) {
    if (!nodo->izq && !nodo->der) { // Tiene 0 hijos
        return 0;
    } else if (nodo->izq && nodo->der) { // Tiene 2 hijos, por ref aux devolvemos el puntero al reemplazante
        *ref_aux = abb_max(&nodo->izq);
        return 2;
    } else { // Tiene 1 hijo, por ref_aux devolvemos el puntero al hijo
        if (nodo->izq) *ref_aux = &nodo->izq;
        else *ref_aux = &nodo->der;
        return 1;
    }
}

nodo_abb_t* nodo_crear(const char *clave, void *dato) {
    char *key = strdup(clave);
    nodo_abb_t* ptr_nodo = malloc(sizeof(nodo_abb_t));
    
    if (ptr_nodo == NULL) {
        free(key);
        return NULL;
    }
    ptr_nodo->clave = key;
    ptr_nodo->dato = dato;
    ptr_nodo->izq = NULL;
    ptr_nodo->der = NULL;
    return ptr_nodo;
}

/*******************************************************************
 *                     PRIMITIVAS DEL ABB
 ******************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	abb_t* abb = malloc(sizeof(abb_t));
	if (abb == NULL) return NULL;

	abb->raiz = NULL;
	abb->cant = 0;
	abb->destruir_dato = destruir_dato;
	abb->comparar = cmp;

	return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    nodo_abb_t** padre = &(arbol->raiz);
    nodo_abb_t* hijo = abb_busqueda(arbol->raiz, arbol->comparar, clave, &padre);
	
    if (hijo == NULL) {	// Si la clave no se encuentra en el arbol simplemente guardamos
		hijo = nodo_crear(clave, dato);
		if (hijo == NULL) return false;
		*padre = hijo; // Actualizamos en el padre la referencia al nuevo nodo
		arbol->cant++;
    } else {	// Si la clave ya esta en el arbol, sobreescribimos su valor
		if (arbol->destruir_dato != NULL) arbol->destruir_dato(hijo->dato);
		hijo->dato = dato;
    }
  	return true;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
    nodo_abb_t** padre =&(arbol->raiz);
    nodo_abb_t* nodo = abb_busqueda(arbol->raiz, arbol->comparar, clave, &padre);

    if (nodo == NULL) {
        return NULL;
    }

    void* dato_auxiliar = nodo->dato;
    nodo_abb_t** ref_aux = NULL; // Nodo auxiliar que guarda al reemplazante en el caso 2, o al unico hijo en el caso 1 
    char* clave_reemplazante;
    char* dato_reemplazante;

    switch (cant_hijos(nodo, &ref_aux)) {
    	case 0:	// El nodo a borrar no tiene hijos
			*padre = NULL;
			free(nodo->clave);
			free(nodo);
			arbol->cant--;
			break;

        case 1:	// El nodo a borrar tiene un solo hijo
			*padre = *ref_aux;
			free(nodo->clave);
			free(nodo);
			arbol->cant--;
        	break;

        case 2: // El nodo a borrar tiene dos hijos
            clave_reemplazante = strdup((*ref_aux)->clave);
            dato_reemplazante = (*ref_aux)->dato;
            abb_borrar(arbol, clave_reemplazante);
            free(nodo->clave);
            nodo->dato = dato_reemplazante;
            nodo->clave = clave_reemplazante;
            break;

        default:
            break;
    }
    return dato_auxiliar;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    nodo_abb_t* nodo = abb_busqueda(arbol->raiz, arbol->comparar, clave, NULL);
    return (nodo != NULL? nodo->dato: NULL);
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    return (abb_busqueda(arbol->raiz, arbol->comparar, clave, NULL) != NULL);
}

size_t abb_cantidad(const abb_t *arbol) {
	return arbol->cant;
}

// Funcion recursiva
void _abb_destruir(nodo_abb_t* nodo, abb_destruir_dato_t destruir) {
	if (nodo == NULL) return;
	if (destruir != NULL) destruir(nodo->dato);

	_abb_destruir(nodo->izq, destruir);
	_abb_destruir(nodo->der,destruir);
	free(nodo->clave);
	free(nodo);
 }

// Funcion wrapper
void abb_destruir(abb_t *arbol) {
	_abb_destruir(arbol->raiz, arbol->destruir_dato);
	free(arbol);
}

/*******************************************************************
 *                PRIMITIVAS DEL ITERADOR INTERNO
 ******************************************************************/

// Funcion recursiva
void _abb_in_order(nodo_abb_t *nodo, bool visitar(const char *, void *, void *), void *extra, bool* aux) {
	if (nodo == NULL) {
		return;
	}
	_abb_in_order(nodo->izq, visitar, extra, aux);
	if (*aux == true) {
		*aux = visitar(nodo->clave, nodo->dato, extra);
	}
	if (*aux == true) {
		_abb_in_order(nodo->der, visitar, extra, aux);
	}
	return;
}

// Funcion wrapper
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	bool aux = true;
	_abb_in_order(arbol->raiz, visitar, extra, &aux);
}



/*******************************************************************
 *             FUNCIONES AUXILIARES DEL ITER EXTERNO
 ******************************************************************/

void apilar_izq(nodo_abb_t* nodo, pila_t* pila) {
	if (nodo == NULL) {
        return;
    }
	pila_apilar(pila, nodo);
    apilar_izq(nodo->izq, pila);
}

/*******************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 ******************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
	abb_iter_t* ptr_iter = malloc(sizeof(abb_iter_t));
	if (ptr_iter == NULL) return NULL;

	ptr_iter->pila = pila_crear();
	if (ptr_iter->pila == NULL) return NULL;

	// Apilo raíz y todos los hijos izquierdos
	apilar_izq(arbol->raiz, ptr_iter->pila);
	return ptr_iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
	if (abb_iter_in_al_final(iter)) {
		return false;
	}
	nodo_abb_t* nodo = pila_desapilar(iter->pila);

	// Apilo hijo derecho del desapilado (si existe) y todos los hijos izquierdos
	apilar_izq(nodo->der, iter->pila);
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
	if (abb_iter_in_al_final(iter)) {
		return NULL;
	}
	nodo_abb_t* nodo = pila_ver_tope(iter->pila);
	if (nodo == NULL) return NULL;

	return nodo->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return (pila_esta_vacia(iter->pila));
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	pila_destruir(iter->pila);
	free(iter);
}
