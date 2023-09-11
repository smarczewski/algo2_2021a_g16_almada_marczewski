#define _POSIX_C_SOURCE 200809L
#include "lista.h"
#include "heap.h"
#include "cms.h"
#include "strutil.h"
#include "set_strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definición del struct hashtag.
 */
typedef struct hashtag {
  char* palabra;
  size_t frecuencia;
} hashtag_t;

/* ******************************************************************
 *                   FUNCIONES AUXILIARES
 * *****************************************************************/

/* Recibe un string que es el tag y la frecuencia del mismo, devuelve una struct
con los dos */
hashtag_t* crear_hashtag(char* palabra, size_t frecuencia) {
  hashtag_t* hashtag = malloc(sizeof(hashtag_t));

  if (hashtag == NULL) return NULL;

  hashtag->palabra = palabra;
  hashtag->frecuencia = frecuencia;

  return hashtag;
}

/* La función de comparar que iría en el heap. Devuelve:
 *   menor a 0  si  a < b
 *   mayor a 0  si  a > b
 *   si son iguales compara alfabeticamente ambos hashtags
 */
int comparar_ht(const void *a, const void *b) {
    hashtag_t* ht_1 = (hashtag_t*)a;
    hashtag_t* ht_2 = (hashtag_t*)b;

    if (ht_1->frecuencia > ht_2->frecuencia) {
        return 1;
    } else if (ht_2->frecuencia > ht_1->frecuencia) {
        return -1;
    } else {
        return strcmp(ht_1->palabra, ht_2->palabra) * -1;
    }
}

// Funcion procesa un arreglo con los tags contenidos en una linea
// es decir, cuenta el hashtag en la matriz CMS y lo agrega al set de tags
void procesar_linea(char** strv, cms_t* cms, set_strings_t* set) {
    for (size_t i = 1; strv[i] != NULL; i++) {
        cms_contar_evento(cms, strv[i]);
        set_insertar(set, strv[i]);
    }
}

// Funcion que arma un heap a partir del set de tags y la matriz CMS
heap_t* armar_heap(set_strings_t* set, cms_t* cms, hashtag_t** arreglo) {
    size_t n = set_largo(set);
    lista_t* tags = set_obtener_lista(set);
    lista_iter_t* iterador = lista_iter_crear(tags); // Crea un iterador para la lista de tags
    size_t pos = 0;

    // Iteramos la lista de hashtags unicos y les asignamos su frecuencia
    while(!lista_iter_al_final(iterador)) {
        char* palabra_actual = lista_iter_ver_actual(iterador);
        size_t freq_min = cms_freq_min(cms, palabra_actual);
        arreglo[pos] = crear_hashtag(palabra_actual, freq_min);
        pos++;
        lista_iter_avanzar(iterador);
    }
    lista_iter_destruir(iterador);
    return heap_crear_arr((void*)arreglo, n, comparar_ht);
}

// Funcion que imprime los top k hashtags, en caso de haber menos
// que k hashtags en el set imprime los que hayan
void imprimir_top_k(cms_t* cms, set_strings_t* set, size_t k) {
    size_t n = k;
    if (set_largo(set) < n) { // Si quedan menos tags que el k solicitado
        n = set_largo(set);
    }
    hashtag_t** arreglo = malloc(set_largo(set) * sizeof(hashtag_t*)); // Arreglo al que le haremos heapify
    heap_t* heap = armar_heap(set, cms, arreglo);

    for (size_t i = 0; i < n; i++) {
        hashtag_t* ht = heap_desencolar(heap);
        printf("%zu %s\n", ht->frecuencia, ht->palabra);
        free(ht);
    }
    heap_destruir(heap, free);
    free(arreglo);
}

/* ******************************************************************
 *                   MAIN PROCESAR TWEETS
 * *****************************************************************/

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Parametros inválidos\n");
        return -1;
    }
    char *end;
    size_t n = (size_t)(strtol(argv[1], &end, 10));
    size_t k = (size_t)(strtol(argv[2], &end, 10));

    char* linea = NULL;
    char** arreglo_linea = NULL;
    size_t tam = 0;

    cms_t* cms = cms_crear(n);
    set_strings_t* set = set_crear();
    size_t contador = 0;
    size_t cont_divs = 1;
    while ((getline(&linea, &tam, stdin)) != EOF) { // En caso de trabajar con stdin, va stdin en vez de archivo
        /* Procesamos los tags */
        arreglo_linea = split(linea, ',');
        procesar_linea(arreglo_linea, cms, set);
        free_strv(arreglo_linea);
        contador++;
        /* Una vez que metimos n lineas al CMS */
        if (contador == n) {
            printf("--- %zu\n", cont_divs);
            cont_divs++; // Aumentamos el contador de ciclos
            imprimir_top_k(cms, set, k);
            set_vaciar(set);
            contador = 0; // Reiniciamos el contador
        } 
    }
    /* Si quedaron tags pero no llegamos a n lineas
     * imprimimos los restantes */
    if (!set_esta_vacio(set)) {
        printf("--- %zu\n", cont_divs);
        imprimir_top_k(cms, set, k);
    }
    set_destruir(set);
    cms_destruir(cms);
    free(linea);
    return 0;
}