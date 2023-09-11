#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calc_helper.h"
#include "strutil.h"

/* Definición del struct operandos, util para manejar con mayor 
facilidad los 1, 2 o 3 posibles operandos.
 */
typedef struct operandos {
    calc_num a;
    calc_num b;
    calc_num c;
}operandos_t;

bool desapilar_necesarios(pilanum_t* pila, enum oper_type operacion, operandos_t* op) {
    bool result = false;
    if (operacion == OP_RAIZ) {
        result = desapilar_num(pila, &op->a);
        return result;
    }
    if (operacion == OP_TERN) {
        desapilar_num(pila, &op->a);
        desapilar_num(pila, &op->b);
        result = desapilar_num(pila, &op->c);
        return result;
    }
    desapilar_num(pila, &op->a);
    result = desapilar_num(pila, &op->b);
    return result;
}

bool oper_validos(operandos_t op, enum oper_type operacion) {
    bool valido = true;
    switch (operacion)
    {
        case OP_DIV: // Division por cero
            if (op.a == 0) valido = false;
            break;
        
        case OP_RAIZ: // Raiz de un numero negativo
            if (op.a < 0) valido = false;
            break;
        
        case OP_POW: // Potencia con exponente negativo
            if (op.a < 0) valido = false;
            break;
        
        case OP_LOG: // Base de logaritmo menor a 2
            if (op.a < 2) valido = false;
            break;

        default:
            break;
    }
    return valido;
}

bool operar(pilanum_t* pila, enum oper_type operacion) {
    operandos_t op;
    calc_num result;
    double aux;

    if (!(desapilar_necesarios(pila, operacion, &op))) { // Si no hay la cant. de operandos necesarios
        return false;
    }
    if (!(oper_validos(op, operacion))) { // Si hay errores propios de c/operacion matematica
        return false;
    }

    switch (operacion)
    {
        case OP_ADD:
            result = op.b + op.a;
            break;
        
        case OP_SUB:
            result = op.b - op.a;
            break;
        
        case OP_MUL:
            result = op.b * op.a;
            break;
        
        case OP_DIV:
            result = op.b / op.a;
            break;
        
        case OP_POW:
            aux = pow((double)op.b, (double)op.a);
            result = (calc_num)aux;
            break;
        
        case OP_RAIZ:
            aux = sqrt((double)op.a);
            result = (calc_num)aux;
            break;
        
        case OP_LOG:
            aux = (log((double)op.b) / log((double)op.a));
            result = (calc_num)aux;
            break;

        case OP_TERN:
            result = op.c ? op.b : op.a;
            break;
        
        default:
            return false;
    }
    apilar_num(pila, result);
    return true;
}

void procesar_arreglo(char** arreglo, pilanum_t* pila) {
    calc_num result = 0;
    token_t token;
    for (size_t i = 0; arreglo[i] != NULL; i++) {
        if (!(calc_parse(arreglo[i], &token))) { // Si hubo un error de parseo
            printf("ERROR\n");
            return;
        }
        if (token.type == TOK_NUM) apilar_num(pila, token.value);
        if (token.type == TOK_OPER) {
            if (!(operar(pila, token.oper.op))) { // Si no se pudo operar
                printf("ERROR\n");
                return;
            }
        }
    }
    desapilar_num(pila, &result);
    if (desapilar_num(pila, &result)) { // Si hay elementos de mas en la pila
        printf("ERROR\n");
        return;
    }
    printf("%ld\n", result);
    return;
}

int main() {
    char* linea = NULL;
    char** arreglo = NULL;
    pilanum_t* pila = NULL;
    size_t tam = 0;

    while ((getline(&linea, &tam, stdin)) != EOF) {
        arreglo = dc_split(linea);
        pila = pilanum_crear();
        procesar_arreglo(arreglo, pila);
        free_strv(arreglo);
        pilanum_destruir(pila);
    }
    
    free(linea);
    return 0;
}


