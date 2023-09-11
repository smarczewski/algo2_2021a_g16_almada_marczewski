#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

#define CAPACIDAD_INICIAL 23
#define FACTOR_CARGA_AGRANDAR 0.65
#define FACTOR_CARGA_ACHICAR 0.22

/* Definición del struct hash y struct campo_hash.
 */
enum estado_hash {
    VACIO,
    OCUPADO,
    BORRADO,
};

typedef struct campo_hash {
    char *clave;
    void* dato;
    enum estado_hash estado;
}campo_hash_t;

struct hash {
    campo_hash_t* tabla;
    size_t tam; // Capacidad de la tabla
    size_t cant;    // Cantidad de elementos en la tabla
    size_t carga;   // Cantidad de elementos ocupados y borrados de la tabla
    hash_destruir_dato_t destruir_dato;
};

/* Definición del iterador externo.
 */
struct hash_iter {
    const hash_t* hash;
    campo_hash_t* actual;
    size_t index; // Indice del campo actual en la tabla
};

/*******************************************************************
 *                FUNCIONES AUXILIARES DEL HASH
 ******************************************************************/

// Funcion hashing Meiyan
size_t funcion_hash(const char *key, size_t tam) {
    int count = (int)strlen(key);
	typedef __uint32_t* P;
	__uint32_t h = 0x811c9dc5;
	while (count >= 8) {
		h = (h ^ ((((*(P)key) << 5) | ((*(P)key) >> 27)) ^ *(P)(key + 4))) * 0xad3e7;
		count -= 8;
		key += 8;
	}
	#define tmp h = (h ^ *(__uint16_t*)key) * 0xad3e7; key += 2;
	if (count & 4) { tmp tmp }
	if (count & 2) { tmp }
    __uint32_t k = (__uint32_t)*key;
	if (count & 1) { h = (h ^ k) * 0xad3e7; }
	#undef tmp
    h = h ^ (h >> 16);
    unsigned int tam_aux = (unsigned int)tam;
    h = h % tam_aux;
	return (size_t)h;
}

// Devuelve la posicion de la tabla en la que vamos a guardar, 
// si devuelve un lugar vacio es, o bien porque vamos a guardar y no hay que reemplazar
// o porque el elemento que buscamos no esta en la tabla
size_t obtener_posicion(campo_hash_t* tabla, const char* clave, size_t tam) {
    size_t pos = funcion_hash(clave, tam);
    while (tabla[pos].estado != VACIO) {    // Iteramos hasta encontrar un vacio
        if (strcmp((tabla[pos].clave), clave) == 0) {
            return pos; // Si no encontramos un vacio pero encontramos la misma clave, la devolvemos
        }
        pos++;
        pos = pos % tam; // Usamos modulo para devolver un indice valido
    }
    return pos;
}

// Inicializa todas las variables de cada elemento de la tabla
void inicializar_tabla(campo_hash_t* tabla, size_t tam) {
    for (size_t i = 0; i < tam; i++) {
        tabla[i].clave = "\0";
        tabla[i].dato = NULL;
        tabla[i].estado = VACIO;
    }
    return;
}

// Funcion auxiliar para redimensionar la tabla
bool hash_redimensionar(hash_t *hash, size_t tam_nuevo) {
    campo_hash_t* tabla_nueva = malloc(tam_nuevo * sizeof(campo_hash_t));
    campo_hash_t* tabla_vieja = hash->tabla;
    size_t tam_viejo = hash->tam;

    if (tam_nuevo > 0 && tabla_nueva == NULL) {
        return false;
    }

    inicializar_tabla(tabla_nueva, tam_nuevo);

    hash->tabla = tabla_nueva;
    hash->tam = tam_nuevo;
    hash->carga = hash->cant; // Los borrados son eliminados y la carga pasa a ser igual a la cantidad de ocupados

    /* Iteramos la tabla vieja y pasamos sus elementos a su nueva posicion en la tabla nueva */
    for (size_t i = 0; i < tam_viejo; i++) {
        if (tabla_vieja[i].estado == OCUPADO) {
            size_t pos = obtener_posicion(hash->tabla, tabla_vieja[i].clave, tam_nuevo);
            hash->tabla[pos].clave = tabla_vieja[i].clave;
            hash->tabla[pos].dato = tabla_vieja[i].dato;
            hash->tabla[pos].estado = OCUPADO;
        }
    }
    free(tabla_vieja);
    return true;
}

 /*******************************************************************
  *                     PRIMITIVAS DEL HASH
  ******************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));

    if (hash == NULL) return NULL;

    hash->tabla = malloc(CAPACIDAD_INICIAL * sizeof(campo_hash_t));

    if (hash->tabla == NULL) {
        free(hash);
        return NULL;
    }

    inicializar_tabla(hash->tabla, CAPACIDAD_INICIAL);

    hash->tam = CAPACIDAD_INICIAL;
    hash->cant = 0;
    hash->carga = 0;
    hash->destruir_dato = destruir_dato;

    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    char *key = strdup(clave);
    double factor = (double)hash->carga / (double)hash->tam;
    if (factor > FACTOR_CARGA_AGRANDAR) {
        size_t cap_nueva = hash->tam * 2;
        if (!(hash_redimensionar(hash, cap_nueva))) {
            free(key);
            return false;
        }
    }
    size_t pos = obtener_posicion(hash->tabla, key, hash->tam);

    if (hash->tabla[pos].estado != VACIO) {
        if (hash->destruir_dato != NULL) hash->destruir_dato(hash->tabla[pos].dato); // Si la clave coincide destruimos el dato viejo
        hash->tabla[pos].dato = dato;
        free(key);
        return true;
    }
    hash->tabla[pos].clave = key;
    hash->tabla[pos].estado = OCUPADO;
    hash->tabla[pos].dato = dato;
    hash->cant++;
    hash->carga++;
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    size_t pos = obtener_posicion(hash->tabla, clave, hash->tam);

    if (hash->tabla[pos].estado == VACIO) { // Si el lugar que le corresponde esta vacio, el elemento no esta
        return NULL;
    }
    char* dato_auxiliar = hash->tabla[pos].dato;

    hash->tabla[pos].estado = BORRADO;
    hash->tabla[pos].dato = NULL;
    free(hash->tabla[pos].clave);
    hash->tabla[pos].clave = "\0";
    hash->cant--;
    
    double factor = (double)hash->cant / (double)hash->tam;
    size_t cap_nueva = hash->tam / 2;
    if ((factor <= FACTOR_CARGA_ACHICAR) && (cap_nueva > CAPACIDAD_INICIAL)) {
        hash_redimensionar(hash, cap_nueva);
    }

    return dato_auxiliar;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    size_t pos = obtener_posicion(hash->tabla, clave, hash->tam);
    if (hash->tabla[pos].estado == VACIO) { // Si el lugar que le corresponde esta vacio, el elemento no esta
        return NULL;
    }
    return hash->tabla[pos].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    size_t pos = obtener_posicion(hash->tabla, clave, hash->tam);
    if (hash->tabla[pos].estado == VACIO) { // Si el lugar que le corresponde esta vacio, el elemento no esta
        return false;
    }
    return true;
}

size_t hash_cantidad(const hash_t *hash) {
    return hash->cant;
}

void hash_destruir(hash_t *hash) {
    for (size_t i = 0; i < hash->tam; i++) {
        if (hash->tabla[i].estado == OCUPADO) {
            if (hash->destruir_dato != NULL) hash->destruir_dato(hash->tabla[i].dato);
            free(hash->tabla[i].clave);
        }
    }
    free(hash->tabla);
    free(hash);
}

/*******************************************************************
 *              FUNCIONES AUXILIARES DEL ITERADOR
 ******************************************************************/

// Funcion que busca el siguiente ocupado en la tabla y actualiza
// el actual e indice del iterador
void buscar_ocupado(hash_iter_t *iter) {
    for (size_t i = iter->index; i < iter->hash->tam; i++) {
        if (iter->hash->tabla[i].estado == OCUPADO) {
            iter->actual = iter->hash->tabla + i;
            iter->index = i;
            return;
        }
    }
    /* Si salimos del loop significa que llegamos al final */
    iter->actual = NULL;
    iter->index = iter->hash->tam;
    return;
}

 /*******************************************************************
  *                PRIMITIVAS DEL ITERADOR DEL HASH
  ******************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t* ptr_iter = malloc(sizeof(hash_iter_t));

    if (ptr_iter == NULL) {
        return NULL;
    }
    ptr_iter->hash = hash;
    ptr_iter->actual = hash->tabla;
    ptr_iter->index = 0;
    
    buscar_ocupado(ptr_iter); // Buscamos el primer ocupado de la tabla

    return ptr_iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) {
        return false;
    }
    iter->index++;
    buscar_ocupado(iter); // Buscamos el siguiente ocupado
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) {
        return NULL;
    }
    return iter->actual->clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
    return iter->actual == NULL;
}

void hash_iter_destruir(hash_iter_t *iter) {
    free(iter);
}