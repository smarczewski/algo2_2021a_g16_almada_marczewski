from grafo import Grafo
from biblioteca import bfs, camino_minimo, camino_minimo_bfs, componentes_fuertemente_conexas
import random

RANGO_DEFAULT = 4
DIVISOR_RANGO = 25
LIMITE_RANGO = 75

# ---------------------------------AUXILIARES----------------------------------


def procesar_linea(archivo):
    """
    Lee un linea de archivo.
    """
    linea = archivo.readline()
    if linea:
        devolver = linea.split("\t")
        return devolver[0], devolver[1].rstrip("\n")
    return None


def procesar_archivo(nombre_archivo):
    """
    Procesa un archivo tsv y devuelve un grafo con los datos del mismo.
    """
    archivo = open(nombre_archivo, "r")
    grafo = Grafo()
    linea = procesar_linea(archivo)

    while linea:
        for v in linea:
            if not grafo.pertenece(v):
                grafo.agregar_vertice(v)
        grafo.agregar_arista(linea[0], linea[1])
        linea = procesar_linea(archivo)

    archivo.close()
    return grafo

# -----------------------------MÍNIMOS SEGUIMIENTOS----------------------------


def min_seguimientos(grafo, origen, destino):
    """
    Imprime una lista con los delincuentes (su código identificador) con los cuáles
    vamos del delincuente origen al delincuente destino de la forma más rápida.
    """
    camino_min = camino_minimo_bfs(grafo, origen, destino)

    if camino_min is not None:
        print("{} " .format(camino_min[0]), end="")

        for v in camino_min[1:]:
            print("-> {} " .format(v), end="")

        print("\r")
    else:

        print("Seguimiento imposible")

# -------------------------DELINCUENTES MAS IMPORTANTES------------------------


def pagerank(grafo):
    """
    Algoritmo de pagerank que realiza una aproximación para determinar los
    delincuentes más importantes.
    """
    pr_actual = {}
    entradas = {}
    gr_salida = {}
    d = 0.85
    vertices = grafo.obtener_vertices()
    n = grafo.cantidad()

    if n > LIMITE_RANGO:
        rango = n // DIVISOR_RANGO
    else:
        rango = RANGO_DEFAULT

    for v in vertices:
        pr_actual[v] = 1/n
        gr_salida[v] = 0
        for w in grafo.adyacentes(v):
            gr_salida[v] += 1
            if w in entradas:
                entradas[w].append(v)
            else:
                entradas[w] = [v]

    for i in range(rango):
        pr_anterior = pr_actual

        for v in vertices:
            pr_actual[v] = 0

            for w in entradas[v]:
                pr_actual[v] += pr_anterior[w]/gr_salida[w]
            pr_actual[v] *= d
            pr_actual[v] += (1-d)/n

    return sorted(pr_actual.items(), key=lambda x: x[1], reverse=True)


def mas_imp(grafo, n, pr):
    """
    Imprime, de mayor a menor importancia, los cant delincuentes más importantes.
    """

    print("{}" .format(pr[0][0]), end="")
    for i in range(1, n):
        print(", {}" .format(pr[i][0]), end="")
    print("\r")

# ------------------------------PERSECUCION RAPIDA-----------------------------


def menor_camino(grafo, delincuentes, k, pr):
    """
    Función auxiliar de persecución_rapida, obtiene el menor camino para llegar de
    uno de los delincuentes pasados por parámetro hasta uno de los K delincuentes
    mas importantes, devuelve el diccionario padres y cual es el inicio y fin.
    """
    # Sacamos los K delincuentes mas importantes
    top_k = pr[:k]

    inicio, fin = 0, 0
    camino = None
    distancia = float('inf')

    # Sacamos camino mínimo de cada delincuente pasado a cada K delincuente
    for j in top_k:

        for i in delincuentes:
            if grafo.pertenece(i):
                # Buscamos el camino mínimo pero cortamos antes en caso de pasarnos de la distancia que tenemos
                camino_aux = camino_minimo(grafo, i, j[0],  distancia)

                if camino_aux is not None:  # Si encontramos un camino mas corto que el teníamos lo guardamos
                    camino = camino_aux
                    distancia = len(camino)
                    inicio, fin = i, j[0]

    return camino, inicio, fin


def persecucion_rapida(grafo, delincuentes, k, pr):
    """
    Dado cada uno de los delincuentes pasados (agentes encubiertos), obtener cuál
    es el camino más corto para llegar desde alguno de los delincuentes pasados por
    parámetro, a alguno de los K delincuentes más importantes. En caso de tener
    caminos de igual largo, priorizar los que vayan a un delincuente más importante.
    """
    camino, inicio, fin = menor_camino(grafo, delincuentes, k, pr)
    if camino is not None:
        print(*camino, sep=" -> ")
    else:
        print("Persecución rápida imposible")

# ---------------------------------COMUNIDADES---------------------------------


def max_freq(grafo, labels, v):
    """
    Función auxiliar de comunidades que toma un grafo, un diccionario de labels y
    un vértice v y devuelve la label que aparece con mas frecuencia entre los
    adyacentes al vértice v.
    """
    freq = {}
    adyacentes = grafo.adyacentes(v)

    # Iteramos los adyacentes a v e inicializamos contadores
    for w in adyacentes:
        freq[labels[w]] = 0

    # Iteramos los adyacentes a v contando la frecuencia de las labels
    for w in adyacentes:
        freq[labels[w]] += 1

    # Obtenemos la label que aparece con mas frecuencia en los adyacentes
    max_label = max(freq, key=freq.get)

    return max_label


def label_propagation(grafo):
    """
    Algoritmo de label propagation que toma un grafo y forma comunidades.
    """
    labels = {}
    vertices = list(grafo.obtener_vertices())
    contador = 0
    cant = grafo.cantidad()

    # Para cada vértice Vi: Label[Vi] = i
    for v in vertices:
        labels[v] = contador
        contador += 1

    # Determinar un orden aleatorio para los vértices
    random.shuffle(vertices)

    # A cada vértice le asignamos la Label que aparece más frecuentemente entre sus adyacentes
    i = 0
    k = 0
    while k < 5:  # Hacemos 5 iteraciones
        labels[vertices[i]] = max_freq(grafo, labels, vertices[i])
        i += 1

        if i == cant:
            i = 0
            k += 1
            random.shuffle(vertices)

    return labels


def armar_comunidades(labels):
    """
    Función auxiliar de comunidades que toma las labels obtenidas con label
    propagation y devuelve un diccionario con las comunidades formadas ordenadas
    de mayor a menor según tamaño.
    """

    comunidades = {}

    # Armamos un diccionario con las labels como key y listas vacías como values
    for label in labels.values():
        comunidades[label] = []

    # Agregamos los vertices en la comunidad que le corresponda según su label
    for v, label in labels.items():
        comunidades[label].append(v)

    # Devolvemos el diccionario ordenado de mayor a menor según cantidad de vertices en la comunidades
    return sorted(comunidades.items(), key=lambda x: len(x[1]), reverse=True)


def imprimir_comunidades(comunidades, n):
    """
    Función auxiliar de comunidades que toma un diccionario con las comunidades
    e imprime las que tengan mas de n miembros siguiendo el formato requerido.
    """
    i = 1
    for comunidad in comunidades:

        if len(comunidad[1]) >= n:
            print("Comunidad ", i, ": ", sep="", end="")
            print(*comunidad[1], sep=", ")
            i += 1

        else:
            # Podemos dejar de iterar ya que el resto de comunidades serán mas chicas
            break


def comunidades(grafo, n):
    """
    Imprime un listado de comunidades de al menos n integrantes.
    """
    labels = label_propagation(grafo)  # Hacemos la propagación de labels

    # Armamos las comunidades según las labels resultantes
    comunidades = armar_comunidades(labels)

    # Imprimimos las comunidades en el formato requerido
    imprimir_comunidades(comunidades, n)

# ----------------------------DIVULGACION DE RUMOR-----------------------------


def divulgar(grafo, delicuente, n):
    """
    Imprime una lista con todos los delincuentes a los cuales les termina
    llegando un rumor que comienza en el delincuente pasado por parámetro, y a lo
    sumo realiza n saltos (luego, se empieza a tergiversar el mensaje), teniendo
    en cuenta que todos los delincuentes transmitirán el rumor a sus allegados.
    """
    padres, orden = bfs(grafo, delicuente)
    ordenados = sorted(orden.items(), key=lambda x: x[1])
    enterados = []
    i = 1

    while ordenados[i][1] <= n:
        enterados.append(ordenados[i][0])
        i += 1
    print("{}" .format(enterados[0]), end="")

    for v in enterados[1:]:
        print(", {}" .format(v), end="")
    print("\r")

# ------------------------------CICLO DE LARGO N-------------------------------


def ciclo_dfs(grafo, v, visitados, camino, x, n):
    """
    Función auxiliar de divulgar ciclo que devuelve True si encontró el camino
    solicitado y False caso contrario.
    """
    if len(camino) == n+1:
        return False

    visitados.add(v)
    camino.append(v)

    for w in grafo.adyacentes(v):
        if w == x and len(camino) == n:
            camino.append(w)
            return True

        if w not in visitados:
            if ciclo_dfs(grafo, w, visitados, camino, x, n):
                return True

    visitados.remove(v)
    camino.pop()
    return False


def divulgar_ciclo(grafo, v, n):
    """
    Permite encontrar un camino simple que empiece y termine en el delincuente
    pasado por parámetro, de largo n.
    """
    camino = []
    visitados = set()

    if ciclo_dfs(grafo, v, visitados, camino, v, n):
        print("{} " .format(camino[0]), end="")

        for v in camino[1:]:
            print("-> {} " .format(v), end="")
        print("\r")

    else:
        print("No se encontro recorrido")

# -----------------------COMPONENTES FUERTEMENTE CONEXAS-----------------------


def cfc(grafo):
    """
    Imprime cada conjunto de vértices entre los cuales todos están conectados con todos.
    """
    # Sacamos CFCs (usando Tarjan)
    cfcs = componentes_fuertemente_conexas(grafo)
    i = 1
    for j in cfcs:  # Imprimimos cada una de las CFC con el formato requerido
        print("CFC ", i, ": ", sep="", end="")
        print(*j, sep=", ")
        i += 1
