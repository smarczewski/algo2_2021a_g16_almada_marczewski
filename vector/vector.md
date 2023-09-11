# **Vector Dinámico**

Deberán leer el archivo `vector.h` con la documentación, e implementar en `vector.c` las cuatro primitivas que faltan:

* `vector_destruir()`
* `vector_obtener()`
* `vector_guardar()`
* `vector_largo()`

Además, deberán estudiar la implementación de las siguientes dos primitivas:

* `vector_crear()`
* `vector_redimensionar()`

La implementación debe respetar los lineamientos de la documentación y tal como lo hicieron con el TP0 deben lograr pasar todas las pruebas con OK.

Usar la misma linea provista para el TP0 para compilar. Para correr Valgrind, usar:

```bash
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
```