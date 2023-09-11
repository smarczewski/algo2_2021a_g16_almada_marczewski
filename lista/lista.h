#include <stdbool.h>
#include <stddef.h>


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

typedef struct lista lista_t;

// Crea una lista enlazada.
// Post: devuelve una nueva lista enlazada vacía.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, falso en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento a la lista en la primera posicion. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Borra el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su dato, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el dato del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el dato del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el dato del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el dato del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el dato del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve el tamaño de la lista
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* ******************************************************************
 *                 PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

// Es un iterador interno que recorre la totalidad de la lista.
// Pre: la lista fue creada. visitar es una función que opera sobre
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: realiza las operaciones indicadas por visitar sobre la lista.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                 PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

typedef struct lista_iter lista_iter_t;

// Crea el iterador externo de la lista.
// Post: devuelve un iterador externo de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza una posicion en la lista enlazada, devuelve falso en caso de llegar al final.
// Pre: el iterador fue creado.
// Post: se avanzo una posicion en el iterador.
bool lista_iter_avanzar(lista_iter_t *iter);

// Muestra el dato de la actual posicion del iterador, devuelve NULL en caso de estar al final.
// Pre: el iterador fue creado.
// Post: se devolvio el elemento de la lista al que apunta el iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve verdadero si el iterador llego al final de la lista. En caso contrario,
// devuelve falso.
// Pre: el iterador fue credo.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se elimino el iterador externo.
void lista_iter_destruir(lista_iter_t *iter);

// Agrega un nuevo elemento a la lista entre la posicion actual del iterador y la anterior.
// Devuelve falso en caso de error.
// Pre: el iterador fue creado.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra entre la posicion actual del iterador 
// y la anterior.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra el elemento de la lista al que apunta el iterador. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su dato, si está vacía, devuelve NULL.
// Pre: el iterador fue creado.
// Post: se devolvió el dato del elemento al que apunta el iterador, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_iter_borrar(lista_iter_t *iter);

/* ******************************************************************
 *                        FUNCION DE PRUEBAS
 * *****************************************************************/

void pruebas_lista_estudiante(void);