#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calc_helper.h"
#include "strutil.h"
#include "cola.h"

bool tope_es_oper(const pila_t *pila) {
    token_t token;
    char* tope = pila_ver_tope(pila);
    if (tope == NULL) {
        return false;
    }
    calc_parse(tope, &token);
    return (token.type == TOK_OPER);
}

bool tope_es_lparen(const pila_t *pila) {
    token_t token;
    char* tope = pila_ver_tope(pila);
    if (tope == NULL) {
        return false;
    }
    calc_parse(tope, &token);
    return (token.type == TOK_LPAREN);
}

unsigned precedencia_tope(const pila_t *pila) {
    token_t token;
    char* tope = pila_ver_tope(pila);
    if (tope == NULL) {
        return false;
    }
    calc_parse(tope, &token);
    return token.oper.precedencia;
}

void imprimir_cola(cola_t *cola) {
    while (!(cola_esta_vacia(cola))) {
        char* operando = cola_desencolar(cola);
        if (operando != NULL) printf("%s ", operando);
    }
    printf("\n");
}

void convertir_arreglo(char** arreglo, cola_t* result, pila_t* pila) {
    token_t token;
    for (size_t i = 0; arreglo[i] != NULL; i++) {
        if (!(calc_parse(arreglo[i], &token))) { // Si hubo un error de parseo
            printf("ERROR\n");
            return;
        }
        if (token.type == TOK_NUM) { // Si el token leido es un numero
            cola_encolar(result, arreglo[i]);
        }
        else if (token.type == TOK_OPER) { // Si el token leido es un operador
            /* Mientras haya un operador en el tope de la pila de operadores y, el operador del tope tenga mayor precedencia
            o bien, tenga igual precedencia y el token leido asocia por izquierda, y ademas
            el operador del tope no es un parentesis izquierdo */
            while ((tope_es_oper(pila)) && ((precedencia_tope(pila) > token.oper.precedencia) || 
                    ((precedencia_tope(pila) == token.oper.precedencia) && (token.oper.asociatividad == ASSOC_L))) 
                        && (token.type != TOK_LPAREN)) {
                char* aux = pila_desapilar(pila);
                cola_encolar(result, aux);
            }
            pila_apilar(pila, arreglo[i]);

        } else if (token.type == TOK_LPAREN) { // Si el token leido es un parentesis izquierdo
            pila_apilar(pila, arreglo[i]);

        } else if (token.type == TOK_RPAREN) { // Si el token leido es un parentesis derecho
            while (!(tope_es_lparen(pila))) {
                char* aux = pila_desapilar(pila);
                cola_encolar(result, aux);
            }
            if (tope_es_lparen(pila)) {
                pila_desapilar(pila);
            }
        }
    }
    while (!(pila_esta_vacia(pila))) {
            char* aux = pila_desapilar(pila);
            cola_encolar(result, aux);
    }
    return;
}

int main(int argc, char* argv[]) {
    char* linea = NULL;
    char** arreglo = NULL;
    cola_t* result = NULL;
    pila_t* operadores = NULL;
    size_t tam = 0;

    while ((getline(&linea, &tam, stdin)) != EOF) {
        arreglo = infix_split(linea);
        result = cola_crear();
        operadores = pila_crear();
        convertir_arreglo(arreglo, result, operadores); // Convertimos el arreglo
        imprimir_cola(result); // Imprimimos la linea convertida
        free_strv(arreglo);
        pila_destruir(operadores);
        cola_destruir(result, NULL);
    }
    
    free(linea);
    return 0;
}