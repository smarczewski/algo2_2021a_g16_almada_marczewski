#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Definición del struct set de strings.
 */
typedef struct set_strings {
    lista_t* tags_unicos;
    hash_t* pertenece;
} set_strings_t;

/* ******************************************************************
 *                 PRIMITIVAS DEL SET DE STRINGS
 * *****************************************************************/

set_strings_t *set_crear(void) {
    set_strings_t* set = malloc(sizeof(set_strings_t));

    if (set == NULL) return NULL;

    set->tags_unicos = lista_crear();

    if (set->tags_unicos == NULL) {
        free(set);
        return NULL;
    }

    set->pertenece = hash_crear(NULL);

    if (set->pertenece == NULL) {
        free(set->tags_unicos);
        free(set);
        return NULL;
    }

    return set;
}

bool set_insertar(set_strings_t* set, char* string) {
    if (hash_pertenece(set->pertenece, string)) {
        return true;
    }
    char *key = strdup(string);
    if (!hash_guardar(set->pertenece, key, NULL)) {
        free(key);
        return false;
    }
    if (!lista_insertar_ultimo(set->tags_unicos, (void*)key)) {
        free(key);
        return false;
    }
    return true;
}

bool set_pertenece(set_strings_t* set, char* string) {
    if (hash_pertenece(set->pertenece, string)) {
        return true;
    }
    return false;
}

size_t set_largo(const set_strings_t *set) {
    return hash_cantidad(set->pertenece);
}

bool set_esta_vacio(const set_strings_t *set) {
    return (hash_cantidad(set->pertenece) == 0);
}

lista_t* set_obtener_lista(set_strings_t *set) {
    return set->tags_unicos;
}

bool set_vaciar(set_strings_t *set) {
    hash_destruir(set->pertenece);
    lista_destruir(set->tags_unicos, free);

    set->tags_unicos = lista_crear();

    if (set->tags_unicos == NULL) {
        free(set);
        return false;
    }

    set->pertenece = hash_crear(NULL);

    if (set->pertenece == NULL) {
        free(set->tags_unicos);
        free(set);
        return false;
    }

    return true;
}

void set_destruir(set_strings_t *set) {
    lista_destruir(set->tags_unicos, free);
    hash_destruir(set->pertenece);
    free(set);
}