#!/usr/bin/python3

import sys
import utilidades


def main():
    """
    Programa principal, recibe el archivo por parámetro y carga en memoria el set 
    de datos. Luego espera la recepción de comandos por entrada estándar, esto 
    permite tener un archivo de instrucciones a ser ejecutadas.
    """
    n = len(sys.argv)
    if n < 2:
        print("Error: Muy pocos argumentos pasados por linea de comando")

    nombre_archivo = str(sys.argv[1])
    grafo = utilidades.procesar_archivo(nombre_archivo)  # Armamos el grafo

    # Esperamos la recepción de comandos por entrada estándar
    leer_comandos(grafo)


def leer_comandos(grafo):
    """
    Funcion que toma un grafo y toma comandos por stdin, los cuales parsea y 
    ejecuta segun corresponda.
    """
    pr = None # Diccionario pagerank para evitar calcularlo mas de una vez
    for linea in sys.stdin:
        # Parseamos la linea pasada por stdin
        comando, n = obtener_argumentos(linea)
        # Por ahi sacamos esto al final pero es un comando de salida de ejecucion
        if comando[0] == "quit" or n == 0:
            break
        # Ejecutamos, de ser posible, el comando parseado
        pr = ejecutar_comando(grafo, comando, n, pr)


def obtener_argumentos(linea):
    """
    Funcion que toma la linea pasada por stdin y devuelve una lista de los 
    argumentos pasados y la cantidad de los mismos.
    """
    argumentos = linea.rstrip().split()
    if not argumentos:
        # De haberse leido una linea vacia devolvemos una lista vacia
        return [""], 0
    return argumentos, len(argumentos)


def ejecutar_comando(grafo, comando, n, pr):
    """
    Funcion que toma un grafo, una lista con los argumentos pasados por stdin y el 
    numero de argumentos, y ejecuta el comando que corresponda luego de chequear 
    que los parametros pasados son correctos. Ademas devuelve el pagerank del 
    grafo si se calculo.
    """
    if comando[0] == "min_seguimientos" and n == 3:
        # Chequeamos que nos pasen el comando y 2 parametros
        try:
            utilidades.min_seguimientos(grafo, comando[1], comando[2])
        except:
            print("Error: Los parametros pasados son incorrectos")

    elif comando[0] == "mas_imp" and n == 2:
        # Chequeamos que nos pasen el comando y un parametro de tipo entero
        try:
            if pr == None: # Si todavia no se calculo el pr lo hacemos
                pr = utilidades.pagerank(grafo)
            utilidades.mas_imp(grafo, int(comando[1]), pr)
        except:
            print("Error: Los parametros pasados son incorrectos")

    elif comando[0] == "persecucion" and n == 3:
        # Chequeamos que nos pasen el comando y 2 parametros, el segundo es de tipo entero
        # Procesamos el primer parametro (lista de delincuentes)
        delincuentes = lista_delincuentes(comando[1])
        try:
            if pr == None: # Si todavia no se calculo el pr lo hacemos
                pr = utilidades.pagerank(grafo)
            utilidades.persecucion_rapida(grafo, delincuentes, int(comando[n-1]), pr)
        except:
            print("Error: Los parametros pasados son incorrectos")

    elif comando[0] == "comunidades" and n == 2:
        # Chequearmos que nos pasen el comando y un parametro de tipo entero
        try:
            utilidades.comunidades(grafo, int(comando[1]))
        except:
            print("Error: Los parametros pasados son incorrectos")

    elif comando[0] == "divulgar" and n == 3:
        # Chequeamos que nos pasen el comando y 2 paremetros, el ultimo es de tipo entero
        try:
            utilidades.divulgar(grafo, comando[1], int(comando[2]))
        except:
            print("Error: Los parametros pasados son incorrectos")

    elif comando[0] == "divulgar_ciclo" and n == 3:
        # Chequeamos que nos pasen el comando y 2 paremetros, el ultimo es de tipo entero
        try:
            utilidades.divulgar_ciclo(grafo, comando[1], int(comando[2]))
        except:
            print("Error: Los parametros pasados son incorrectos")

    elif comando[0] == "cfc" and n == 1:
        # Chequeamos que nos pasen el comando solamente
        try:
            utilidades.cfc(grafo)
        except:
            print("Error: Los parametros pasados son incorrectos")

    else:
        print("Error: Comando o parametros invalido/s")

    return pr


def lista_delincuentes(delincuentes):
    """
    Funcion que toma un string con los delincuentes separados por comas y devuelve 
    una lista con los mismos.
    """
    lista = delincuentes.split(',')
    return lista


main()
