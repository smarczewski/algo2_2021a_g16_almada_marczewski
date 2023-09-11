from collections import deque

# ---------------------------------RECORRIDOS----------------------------------


def bfs(grafo, origen):
    """
    Recorre el grafo por Breadth First Search partiendo desde el vértice origen
    """
    visitados = set()
    padres = {}
    orden = {}
    visitados.add(origen)
    padres[origen] = None
    orden[origen] = 0
    cola = deque()
    cola.append(origen)

    while len(cola) != 0:
        v = cola.popleft()
        for w in grafo.adyacentes(v):
            if w not in visitados:
                padres[w] = v
                orden[w] = orden[v] + 1
                visitados.add(w)
                cola.append(w)
    return padres, orden

# ----------------------COMPONENTES FUERTEMENTES CONEXOS-----------------------


def componentes_fuertemente_conexas(grafo):
    """
    Función que toma un grafo dirigido y devuelve una lista de todas sus componentes
    fuertemente conexas.
    """

    orden_contador = [0]  # Contador "global"
    v = list(grafo.obtener_vertices())[0]  # Primer vértice del grafo
    visitados = set()
    apilados = set()
    pila = deque()
    orden = {}
    mb = {}
    todas_cfc = []  # Lista de todas las CFC
    orden[v] = orden_contador[0]  # Inicializamos el orden del primer vértice

    # Algoritmo de Tarjan mostrado en el canal de la cátedra
    def cfc_tarjan(v):
        visitados.add(v)
        pila.appendleft(v)
        apilados.add(v)
        mb[v] = orden[v]

        for w in grafo.adyacentes(v):
            if w not in visitados:
                orden_contador[0] += 1
                orden[w] = orden_contador[0]
                cfc_tarjan(w)
                mb[v] = min(mb[v], mb[w])

            elif w in apilados:
                mb[v] = min(mb[v], orden[w])

        if orden[v] == mb[v] and len(pila) > 0:
            nueva_cfc = []
            while True:
                w = pila.popleft()
                apilados.remove(w)
                nueva_cfc.append(w)
                if w == v:
                    break

            todas_cfc.append(nueva_cfc)

    cfc_tarjan(v)  # Llamamos al algoritmo de Tarjan
    return todas_cfc


# -------------------------------CAMINOS MÍNIMOS-------------------------------

def camino_minimo_bfs(grafo, origen, destino, limite=float("inf")):
    """
    Algoritmo que encuentra el camino mínimo entre dos vertices en un grafo pasado
    por parámetro, toma un argumento opcional para limitar el recorrido a una
    distancia tope. Funciona únicamente para grafos no pesados.
    """
    cola = deque()
    visitado = set()
    distancia = {}
    padre = {}

    grafo_vertices = grafo.obtener_vertices()
    for vertice in grafo_vertices:
        distancia[vertice] = float("inf")

    distancia[origen] = 0
    padre[origen] = None
    visitado.add(origen)

    cola.append(origen)

    while len(cola) != 0:
        v = cola.popleft()
        if v == destino:
            return reconstruir_camino(padre, destino)

        if distancia[v] + 1 >= limite:  # Si ya estamos o nos pasamos del limite
            return None

        for w in grafo.adyacentes(v):
            if w not in visitado:
                distancia[w] = distancia[v] + 1
                padre[w] = v
                visitado.add(w)
                cola.append(w)

            if distancia[w] > distancia[v] + 1:
                distancia[w] = distancia[v] + 1
                padre[w] = v

    return None


def camino_minimo(grafo, v, w, limite=float("inf")):
    """
    Función que devuelve el camino mínimo entre un origen y un destino utilizando
    el algoritmo bfs pero corta el recorrido si nos pasamos del limite pasado por
    parámetro, el cual es opcional.
    """
    if not grafo.pertenece(v) or not grafo.pertenece(w):
        return None

    camino = camino_minimo_bfs(grafo, v, w, limite)

    return camino


def reconstruir_camino(padres, destino):
    """
    Devuelve una lista con los vertices que forman un camino hacía destino
    """

    recorrido = []
    while destino is not None:
        recorrido.append(destino)
        destino = padres[destino]
    return recorrido[::-1]
