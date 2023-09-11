#include "testing.h"

#include <stdio.h>
#include <stdlib.h>

void pruebas_heap_catedra(void);
void pruebas_heap_estudiante(void);
void pruebas_heap_volumen_catedra(size_t, bool);

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

#ifndef CORRECTOR
int main() {
    printf("~~~ PRUEBAS ESTUDIANTE ~~~\n");
    pruebas_heap_estudiante();

    return failure_count() > 0;
}
#endif