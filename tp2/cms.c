#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "heap.h"

#define FILAS 5
#define TAM_MINIMO 59

/* Tipo de funcion de hashing.
 */
typedef size_t (*f_hash_t)(const char *, size_t);

/* Cada una de las filas asociadas a una función de hashing.
 */
typedef struct fila_frecuencias {
    f_hash_t f;
    size_t* frecuencias;
}fila_frecuencias_t;

/* Matriz de CountMinSketch de 3 filas (y por lo tanto, 3 funciones de hashing)
 * y n columnas (se especifica por parametro).
 */
typedef struct CountMinSketch {
  fila_frecuencias_t* tabla;
  size_t columnas;
} cms_t;

/* *****************************************************************
 *                    FUNCIONES DE HASHING
 * *****************************************************************/

// Meiyan
size_t f_hashing_1(const char *key, size_t tam) {
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

// Jenkins (one-at-a-time version)
size_t f_hashing_2(const char *key, size_t tam) {
    uint32_t hash, i;
    size_t len = strlen(key);
    
    for(hash = i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return ((size_t)hash % tam);
}

// Hash function that uses Horner’s rule to compute a polynomial in x =16
// https://cseweb.ucsd.edu/~kube/cls/100/Lectures/lec16/lec16-16.html
size_t f_hashing_3(const char* key, size_t tam) {
	long hashVal = 0;
	while (*key != '\0') {
		hashVal = (hashVal << 4) + *(key++);
		unsigned long g = hashVal & 0xF0000000L;
		if (g != 0) hashVal ^= g >> 24;
		hashVal &= ~g;
	}
	return hashVal % tam;
}

// SDBM Hash Function
size_t f_hashing_4(const char* key, size_t tam)
{
   unsigned int hash = 0;
   unsigned int i    = 0;
   size_t length = strlen(key);

   for (i = 0; i < length; ++key, ++i)
   {
      hash = (*key) + (hash << 6) + (hash << 16) - hash;
   }

   return hash % tam;
}

// MurmurHash (one-byte-at-a-time version)
size_t f_hashing_5(const char *key, size_t tam) {
    uint32_t h = 3323198485ul;
    uint32_t k;
    for (;*key;++key) {
        k = ((uint32_t)*key);
        h ^= k;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h % tam;
}

/* *****************************************************************
 *                      AUXILIARES DEL CMS
 * *****************************************************************/

/* Funcion auxiliar que inicializa los contadores
 */
void inicializar_frecuencias(size_t* frecuencias, size_t columnas) {
    for (size_t i = 0; i < columnas; i++) {
        frecuencias[i] = 0;
    }
}

/* *****************************************************************
 *                      PRIMITIVAS DEL CMS
 * *****************************************************************/

cms_t *cms_crear(size_t n) {
    cms_t* cms = malloc(sizeof(cms_t));

    if (cms == NULL) return NULL;

    cms->tabla = malloc(FILAS * sizeof(fila_frecuencias_t));

    if (cms->tabla == NULL) {
        free(cms);
        return NULL;
    }
    cms->columnas = n / FILAS;
    if (cms->columnas < TAM_MINIMO) cms->columnas = TAM_MINIMO;

    for (size_t i = 0; i < FILAS; i++) {
        cms->tabla[i].frecuencias =  malloc(cms->columnas * sizeof(size_t));
        inicializar_frecuencias(cms->tabla[i].frecuencias, cms->columnas);
    }
    
    cms->tabla[0].f = f_hashing_1;
    cms->tabla[1].f = f_hashing_2;
    cms->tabla[2].f = f_hashing_3;
    cms->tabla[3].f = f_hashing_4;
    cms->tabla[4].f = f_hashing_5;

    return cms;
}

void cms_contar_evento(cms_t* cms, char* evento) {
    size_t pos;
    for (size_t i = 0; i < FILAS; i++) {
        pos = cms->tabla[i].f(evento, cms->columnas);
        cms->tabla[i].frecuencias[pos]++;
    }
}

size_t cms_freq_min(cms_t* cms, char* evento) {
    size_t pos = cms->tabla[0].f(evento, cms->columnas);
    size_t freq = cms->tabla[0].frecuencias[pos];
    for (size_t i = 1; i < FILAS; i++) {
        pos = cms->tabla[i].f(evento, cms->columnas);
        if (cms->tabla[i].frecuencias[pos] < freq) {
            freq = cms->tabla[i].frecuencias[pos];
        }
    }
    return freq;
}

void cms_destruir(cms_t *cms) {
    for (size_t i = 0; i < FILAS; i++) {
        free(cms->tabla[i].frecuencias);
    }    
    free(cms->tabla);
    free(cms);
}