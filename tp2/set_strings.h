#include <stdbool.h>
#include <stddef.h>

struct set_strings;
typedef struct set_strings set_strings_t;

/* ******************************************************************
 *                 PRIMITIVAS DEL SET DE STRINGS
 * *****************************************************************/

/* Crea el set de strings
 */
set_strings_t *set_crear(void);

// Agrega un nuevo string al set. Devuelve falso en caso de error.
// Pre: el set fue creado.
// Post: se agregó un nuevo string unico al set.
bool set_insertar(set_strings_t* set, char* string);

// Determina si el string pertenece al set.
// Pre: el set fue creado.
bool set_pertenece(set_strings_t* set, char* string);

// Devuelve el tamaño del set
// Pre: el set fue creado.
size_t set_largo(const set_strings_t *set);

// Devuelve true si el set esta vacio, false de no estarlo
// Pre: el set fue creado.
bool set_esta_vacio(const set_strings_t *set);

// Vacia el set.
// Pre: el set fue creado.
// Post: el set ahora se encuentra vacio.
bool set_vaciar(set_strings_t *set);

// Devuelve la lista de tags unicos.
// Pre: el set fue creado.
lista_t* set_obtener_lista(set_strings_t *set);

// Destruye el set.
// Pre: el set fue creado.
// Post: se eliminaron todos los elementos del set.
void set_destruir(set_strings_t *set);