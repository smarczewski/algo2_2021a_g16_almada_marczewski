import random


class Grafo(object):
    """Grafo implementado como un diccionario de diccionarios, las claves del primer diccionario son los vertices del
     grafo y su dato un diccionario con sus vertices adyacentes y el peso de las aristas con las que se conectan"""

    def __init__(self, es_dirigido=True, lista_vertices=[]):
        """Creación de un grafo.Sus parámetros son:
             es_dirigido que si no se ingresa ninguno es True
             lista_vertices para agregar al grafo, por defecto es vacía
              Post: el grafo es creado"""

        self.diccionario = {}
        self.dirigido = es_dirigido

        for vertice in lista_vertices:
            self.agregar_vertice(vertice)

    def agregar_vertice(self, v):
        """Agrega v como vértice del grafo
            Pre: el grafo fue creado
            Post: Se agrego v como vértice. Devuelve False si el elemento ya estaba en el
            grafo, True si no"""

        if v not in self.diccionario:
            self.diccionario[v] = {}
            return True
        return False

    def borrar_vertice(self, v):
        """Borrar el vértice v del grafo
        Pre: el grafo fue creado
        Post: El vértice fue borrado. Devuelve True si el vértice
        se encontraba en el grafo, False en caso contrario"""

        if v in self.diccionario:
            if self.dirigido:
                for clave in self.diccionario:
                    self.diccionario[clave].pop(v)
            else:
                for clave in self.diccionario[v]:
                    self.diccionario[clave].pop(v)
            self.diccionario.pop(v)
            return True

        return False

    def agregar_arista(self, v, w, peso=1):
        """Agrega una arista al grafo que conecta v con w con su peso que por defecto es 1
        Pre: el grafo fue creado
        Post: agrega una arista al grafo conectando v con w, si la arista ya estaba en el grafo,
        actualiza su peso, si v y w no eran vertices del grafo devuelve False, True en caso contrario"""

        if v in self.diccionario and w in self.diccionario:
            self.diccionario[v][w] = peso

            if not self.dirigido:
                self.diccionario[w][v] = peso
            return True
        return False

    def borrar_arista(self, v, w):
        """Borra una arista del grafo
        Pre: el grafo fue creado
        Post: borra la arista que conecta v con w, si no existía tal arista o v o w no se
        encontraban en el grafo, devuelve False, True en caso contrario"""

        if v in self.diccionario and w in self.diccionario[v]:
            self.diccionario[v].pop(w)

            if not self.dirigido:
                self.diccionario[w].pop(v)
            return True

        return False

    def obtener_vertices(self):
        """Devuelve los vertices de grafo
        Pre: El grafo fue creado
        Post: devuelve una lista con los vertices del grafo"""

        return self.diccionario.keys()

    def vertice_aleatorio(self):
        """Devuelve un vértice cualquiera del grafo.
        Pre: el grafo fue creado
        Post: devuelve un vértice aleatorio del grafo, None si no había vertices"""

        random.choice(list(self.diccionario.keys()))

    def adyacentes(self, v):
        """Devuelve los adyacentes del vértice v
        Pre: el grafo fue creado
        Post: devuelve una lista con los vertices adyacentes a v en grafo, None en caso
        v no se encontrara en el grafo"""
        if v in self.diccionario:
            return self.diccionario[v].keys()
        return []

    def peso(self, v, w):
        """Devuelve el peso de la arista v, w
        Pre: el grafo fue creado
        Post: devuelve el peso de la arista que conecta v con w, 0 si no se encontraban
        en el grafo o no estaban unidos"""

        if v in self.diccionario and w in self.diccionario[v]:
            return self.diccionario[v][w]
        return 0

    def cantidad(self):
        """Devuelve la cantidad de elementos en el grafo
        Pre: el grafo fue creado
        Post: devuelve la cantidad de elementos en el grafo"""

        return len(self.diccionario)

    def pertenece(self, v):
        """Determina si el elemento se encuentre en el grafo
        Pre: el grafo fue creado
        Post : devuelve True si el elemento estaba en el grafo, False en caso contrario"""

        return v in self.diccionario

    def esta_unido(self, v, w):
        """Determina si v y w están conectados por una arista
        Pre : el grafo fue creado
        Post: devuelve True si v y w estaban conectados, False en caso contrario"""

        return w in self.diccionario[v]
