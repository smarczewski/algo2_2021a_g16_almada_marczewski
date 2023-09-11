#include <stdbool.h>
#include <stddef.h>

// el struct del ABB debera llamarse "abb"
typedef struct abb abb_t;

// tipo de funcion para comparar claves
typedef int (*abb_comparar_clave_t) (const char *, const char *);

// tipo de funcion para destruir datos
typedef void (*abb_destruir_dato_t) (void *);

/* Primitivas del ABB */

/* Crea el ABB
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el arbol, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura abb fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del ABB y devuelve el dato asociado. Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura abb fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del ABB, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura abb fue inicializada
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si clave pertenece o no al ABB.
 * Pre: La estructura abb fue inicializada
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del ABB.
 * Pre: La estructura abb fue inicializada
 */
size_t abb_cantidad(const abb_t *arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura abb fue inicializada
 * Post: La estructura abb fue destruida
 */
void abb_destruir(abb_t *arbol);


/* Iterador interno del ABB */

// Es un iterador interno que recorre la totalidad del ABB in-order.
// Pre: el ABB fue creado. visitar es una función que opera sobre
// los datos del arbol, o NULL en caso de que no se la utilice.
// Post: realiza las operaciones indicadas por visitar sobre el arbol.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* Iterador externo del ABB */

// el struct del iterador externo debera llamarse "abb_iter"
typedef struct abb_iter abb_iter_t;

// Crea iterador
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza iterador
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Comprueba si terminó la iteración
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye iterador
void abb_iter_in_destruir(abb_iter_t* iter);
