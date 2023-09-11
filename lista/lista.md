# **Lista enlazada**

Se incluye en los archivos un ejemplo de uso de iteradores externos.
Estas son las primitivas de listas que tienen que implementar.
En esta entrega les agregamos el requerimiento de escribir la documentación completa de las primitivas, con sus correspondientes pre y post condiciones, para esto pueden usar de muestra los archivos .h que ya utilizaron para la implementación de pilas y colas.

#### Primitivas de la lista

``` cpp
typedef struct lista lista_t;
lista_t *lista_crear(void);
bool lista_esta_vacia(const lista_t *lista);
bool lista_insertar_primero(lista_t *lista, void *dato);
bool lista_insertar_ultimo(lista_t *lista, void *dato);
void *lista_borrar_primero(lista_t *lista);
void *lista_ver_primero(const lista_t *lista);
void *lista_ver_ultimo(const lista_t* lista);
size_t lista_largo(const lista_t *lista);
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));
```

#### Primitiva del iterador interno

``` cpp
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);
```

#### Primitivas del iterador externo

``` cpp
typedef struct lista_iter lista_iter_t;
lista_iter_t *lista_iter_crear(lista_t *lista);
bool lista_iter_avanzar(lista_iter_t *iter);
void *lista_iter_ver_actual(const lista_iter_t *iter);
bool lista_iter_al_final(const lista_iter_t *iter);
void lista_iter_destruir(lista_iter_t *iter);
bool lista_iter_insertar(lista_iter_t *iter, void *dato);
void *lista_iter_borrar(lista_iter_t *iter);
```

#### Función de pruebas

``` cpp
void pruebas_lista_alumno(void);
```

_Aclaración_: mantener el nombre de la estructura como `struct lista` para la lista, y `struct lista_iter` para el iterador, puesto que el corrector automático tiene configurados dichos nombres en el `lista.h` de la cátedra.
Considerar que todas las primitivas (exceptuando `lista_destruir` y `lista_iterar`) deben funcionar en tiempo constante.
Las pruebas deben incluir los casos básicos de TDA similares a los contemplados para la pila y la cola, y adicionalmente debe verificar los siguientes casos del iterador externo:
1. Al insertar un elemento en la posición en la que se crea el iterador, efectivamente se inserta al principio.
1. Insertar un elemento cuando el iterador está al final efectivamente es equivalente a insertar al final.
1. Insertar un elemento en el medio se hace en la posición correcta.
1. Al remover el elemento cuando se crea el iterador, cambia el primer elemento de la lista.
1. Remover el último elemento con el iterador cambia el último de la lista.
1. Verificar que al remover un elemento del medio, este no está.
1. Otros casos borde que pueden encontrarse al utilizar el iterador externo.
Y los casos con / sin corte del iterador interno.

---
### FAQ

#### ¿Cuándo se define que el iterador externo está al final?

Una pregunta usual a la hora de implementar el iterador externo, es cuál debe ser la condición para definir si el iterador se encuentra al final:
- Cuando el `actual` es igual a `NULL`.
- Cuando el `actual` es igual al `último` de la lista.

La idea del iterador externo es que nos permita pasar por todos los elementos de la lista de forma cómoda. Una forma usual de utilizarlo es dentro de un ciclo, ya sea `while`, o `for`:
```cpp
  lista_iter_t* iter = lista_iter_crear(lista);
  while (!lista_iter_al_final(iter)) {
      void* dato = lista_iter_ver_actual(iter):
      // Realizamos alguna operación sobre el dato
      lista_iter_avanzar(iter);
  }
  lista_iter_destruir(iter);
```
```cpp
  lista_iter_t* iter;
  for (iter = lista_iter_crear(lista); !lista_iter_al_final(iter); lista_iter_avanzar(iter)) {
      void* dato = lista_iter_ver_actual(iter):
      // Realizamos alguna operación sobre el dato
  }
  lista_iter_destruir(iter);
```

En cualquiera de los dos casos, que son equivalentes, es importante notar que el ciclo debe poder pasar por todos los elementos de la lista.

Analizando las dos opciones que tenemos, podemos ver que la segunda va a dar por terminado el ciclo sin haber entrado para el último elemento (una forma fácil de corroborarlo, es revisar el caso de una lista con un sólo elemento y hacer el seguimiento). Por ésto, la solución correcta es la primera: el iterador se encontrará al final cuando ya haya pasado por todos los elementos, lo cual quiere decir que el puntero `actual` es `NULL`.

#### ¿Cómo se comporta el agregado y la eliminación en una lista enlazada con iteradores?

La inserción en una lista enlazada sigue dos reglas:
  - El elemento insertado va a tomar la posicion del elemento al que se apunta.
  - Luego de una insercion, el iterador va a apuntar al nuevo elemento.
Esto no quiere decir que `lista_insertar` sobreescriba datos; sino que si el actual del iterador es el segundo elemento de la lista, realizar una inserción colocaría un nuevo dato entre el elemento uno y el dos:
**Inserciones en una lista**
Dos implicaciones que surgen son:
  - Si se inserta un elemento con un iterador apuntando al primer elemento de la lista, la operación es idéntica a llamar a `lista_insertar_primero`.
  - Si se inserta un elemento con un iterador apuntando al final, la operación es idéntica a llamar a `lista_insertar_ultimo`.
La eliminación funciona de manera análoga: se elimina el elemento al que está apuntando el iterador y este apunta al elemento siguiente:
**Eliminaciones en una lista**
Considerando esto resulta:
  - La eliminación de un elemento con un iterador apuntando al primer elemento de la lista es idéntica a `lista_borrar_primero`.
  - La eliminacion de un elemento con un iterador apuntando al último elemento de la lista (esto es: al elemento tal que cuando se avanza el iterador está al final) se comportaría como una primitiva `lista_borrar_ultimo`. Eliminar este elemento haría que el iterador apunte al final de la lista.
#### ¿Cómo se comporta el iterador interno?
El iterador interno es una herramienta para poder procesar los elementos de la lista sin necesidad de manejar la memoria del iterador externo.
Para hacer esto es necesario escribir la función visitar en donde se especifique qué es lo que se va a hacer con estos nodos:
``` cpp
bool visitar(void* dato, void* extra);
```
Esta función va a ser llamada inicialmente con el dato que ocupa la primera posición de la lista. Como esta función va a ser llamada de manera automática por la primitiva lista_iterar, además debe recibir un puntero extra que puede ser usado para mantener una memoria entre las las sucesivas llamadas.
Adicionalmente, esta función devuelve un valor booleano. Si en algún momento se devuelve false, la iteración se interrumpiría automáticamente.
#### ¿Puedo insertar con el iterador externo y con las primitivas de la lista a la vez?
Si bien poder se puede, esto no es algo que se deba hacer. Al crear un iterador externo de cualquier estructura, comienza un pacto entre el programador y el usuario, y este implica que si creé un iterador externo, es para usarlo. Intentar usar ambas formas de insertar/eliminar 'a la vez' (mientras 'viva' un iterador externo), es romper este pacto y puede generar comportamiento no esperado del lado de la estructura que estoy usando.

---
### Bibliografia recomendada
* Weiss, Mark Allen, "Data Structures and Algorithm Analysis": *3.2. The List ADT*.
* Cormen, Thomas H. "Introduction to Algorithms": *10.2. Linked lists*.