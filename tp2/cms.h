#include <stdbool.h>
#include <stddef.h>

struct cms;
typedef struct cms cms_t;

/* Crea el CMS
 */
cms_t *cms_crear(size_t n);

/* Cuenta en el CMS un evento segun corresponda.
 * Pre: La estructura CMS fue inicializada
 * Post: Se aumentaron los contadores que correspondan
 */
void cms_contar_evento(cms_t* cms, char* evento);

/* Devuelve la frecuencia minima de un evento
 * Pre: La estructura CMS fue inicializada y el evento existe
 */
size_t cms_freq_min(cms_t* cms, char* evento);

/* Destruye la estructura liberando la memoria pedida.
 * Pre: La estructura CMS fue inicializada
 * Post: La estructura CMS fue destruida
 */
void cms_destruir(cms_t *cms);