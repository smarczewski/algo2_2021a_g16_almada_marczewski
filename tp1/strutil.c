#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *substr(const char *str, size_t n) {
    char *cadena = strndup(str, n);
    return cadena;
}

char **split(const char *str, char sep) {
    char *cadena = strdup(str);

    size_t largo = 2; // Porque siempre el nro de cadenas es 1+ que el de seps y 1 extra por la cadena NULL
    for (size_t i = 0; cadena[i]; i++) {
        if (cadena[i] == sep) largo ++;
    }

    char **arreglo = malloc(largo * sizeof(char*));
    size_t j = 0;
    size_t sep_ant = 0;

    for (size_t i = 0; cadena[i]; i++) {
        if (cadena[i] == sep) {
            arreglo[j] = substr(cadena + sep_ant, i - sep_ant);
            sep_ant = i + 1;
            j++;
        }
    }
    arreglo[j] = strdup(cadena + sep_ant);
    arreglo[j+1] = NULL;
    free(cadena);
    return arreglo;
}

char *join(char **strv, char sep) {
    if ((strv == NULL) || (strv[0] == NULL)) {
        char *cadena = strdup("");
        return cadena;
    }
    if (strv[1] == NULL) {
        char *cadena = strdup(strv[0]);
        return cadena;
    }
    size_t largo = 0;
    for (size_t i = 0; strv[i] != NULL; i++) {
        largo += strlen(strv[i]) + 1;
    }
    char sep_str[2] = {sep, '\0'};
    char *cadena = malloc(largo * sizeof(char));
    char *ptr_final = stpcpy(cadena, strv[0]);
    ptr_final = stpcpy(ptr_final, sep_str);

    for (size_t i = 1; strv[i] != NULL; i++) {
        ptr_final = stpcpy(ptr_final, strv[i]);
        if (strv[i+1] != NULL) ptr_final = stpcpy(ptr_final, sep_str);
    }
    return cadena;
}

void free_strv(char *strv[]) {
    for (size_t i = 0; strv[i] != NULL; i++) {
        free(strv[i]);
    }
    free(strv);
}
