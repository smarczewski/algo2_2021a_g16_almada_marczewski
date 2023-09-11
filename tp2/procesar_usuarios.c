#define _POSIX_C_SOURCE 200809L
#include "strutil.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_CHAR_USUARIO 48
#define MAX_CHAR_USUARIO 122
#define MAX_LEN_USUARIO 15

void dato_destruir(void* dato) {
	hash_destruir((hash_t*) dato);
}

/* ******************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/

/* Funcion que procesa una linea, es decir, toma el nombre de un usuario y lo
 * agrega al hash de usuarios unicos, en el cual a c/usuario le corresponde
 * un hash que lleva la cuenta de hashtags unicos que utilizo
 */
void procesar_linea(char *linea, hash_t *hash, size_t *maximo) {
	char **strv = split(linea, ',');
	char *usuario = strv[0]; // El usuario es siempre el primer elemento del arreglo
	hash_t *hash_usuario;

	if (!hash_pertenece(hash, usuario)) { // Si el usuario leido no esta en el hash general
		hash_usuario = hash_crear(NULL); // Creamos un hash para el usuario leido
		hash_guardar(hash, usuario, hash_usuario); // Guardamos el usuario y su hash en el hash general
	} else {
		hash_usuario = hash_obtener(hash, usuario); // Si el usuario leido ya esta en el hash general obtenemos su hash propio
	}

	for (size_t i = 1; strv[i] != NULL; i++) { // Iteramos los tags restantes de la linea
		if (!hash_pertenece(hash_usuario, strv[i])) { // Si el tag leido no esta en el hash del usuario
			hash_guardar(hash_usuario, strv[i], NULL); // Lo guardamos
		}
	}
	if (hash_cantidad(hash_usuario) > *maximo)
		*maximo = hash_cantidad(hash_usuario); // Actualizamos el puntero a maximo si corresponde

	free_strv(strv); // Liberamos el arreglo
}

/* Funcion que se fija si la posicion solicitada en pos existe en el
 * el string que se pasa por parametro, es decir si el string es mas
 * largo que la pos solicitada. De existir se devuelve el caracter que
 * esta en esa posicion en ascii, caso contrario devuelve 0.
 */
size_t ascii(char* usuario, hash_t* hash, size_t min, size_t pos) {
	if (strlen(usuario) > pos) {
		size_t numero = (size_t) usuario[pos];
		return numero-min;
	}
	return 0;
}

/* Funcion que devuelve la cantidad de hashtags unicos que utilizo un 
 * usuario.
 */
size_t cant_tags(char* usuario, hash_t* hash, size_t min, size_t pos) {
	hash_t* hash_usuario = hash_obtener(hash, usuario);
	return hash_cantidad(hash_usuario)-min;
}

/* Funcion que ordena un arreglo de usuarios in-place segun un caracter
 * en especifico, o segun la cantidad de hashtags unicos de c/usuario
 */
void counting_sort(char **array, size_t valor_posicion(char* usuario, hash_t* hash, size_t min, size_t pos),  
					hash_t* hash, size_t cantidad, size_t min, size_t max, size_t pos) {
						
  	size_t k = max - min; // Rango de los digitos
	size_t contador[k + 1];
	size_t suma_acumulada[k + 1];
	suma_acumulada[0] = 0;

	/* Inicializamos contadores */
	for (size_t i = 0; i < k + 1; i++) {
		contador[i] = 0;
	}

	/* Iteramos el arreglo original de usuarios
	y aumentamos contadores segun corresponda */
	for (size_t i = 0; i < cantidad; i++) {
		size_t numero = valor_posicion(array[i], hash, min, pos);
    	contador[numero]++;
	}

	/* Completamos el arreglo de sumas acumuladas */
	for (size_t i = 1; i < (k + 1); i++) {
		size_t n = contador[i - 1];
		suma_acumulada[i] = suma_acumulada[i - 1] + n;
	}

	char* resultado[cantidad]; // Arreglo con las posiciones finales

	size_t posicion;
	/* Iteramos el arreglo original */
	for (size_t i = 0; i < cantidad; i++) {
    size_t numero = valor_posicion(array[i], hash, min, pos);
		posicion = suma_acumulada[numero]; // Obtenemos su nueva posicion del arreglo de sumas acumuladas
		suma_acumulada[numero]++; // Ahora aumentamos la suma acumulada ahora que ocupamos esa pos
		resultado[posicion] = array[i]; // Colocamos el elemento en su nueva posicion en el arreglo resultado
	}

	/* Ordenamos el arreglo original con las posiciones finales */
	for (size_t i = 0; i < cantidad; i++) {
		array[i] = resultado[i];
	}
}

/* Funcion radix-sort que ordena el arreglo de usuarios in-place segun
 * orden alfabetico caracter por caracter utilizando counting-sort
 */
void radix_sort(char **array, hash_t* hash, size_t cantidad, size_t tam) {
	for (size_t i = 0; i < tam; i++) {
		counting_sort(array, ascii, hash, cantidad, MIN_CHAR_USUARIO, MAX_CHAR_USUARIO, tam - 1 - i);
	}
}

/* Funcion que ordena e imprime el arreglo de usuarios segun
 * la cantidad de hashtags unicos que utilizo y en caso de tener
 * la misma cantidad los ordena alfabeticamente
 */
void ordenar(hash_t *hash, size_t maximo) {
	if (hash_cantidad(hash) == 0) return;

	hash_iter_t *iterador = hash_iter_crear(hash);
	size_t n = hash_cantidad(hash); // Cantidad de usuarios unicos
	char **nombres_usuarios = malloc(sizeof(char *) * n);

	/* Armamos un arreglo con todos los usuarios unicos */
	for (size_t i = 0; !hash_iter_al_final(iterador); i++) {
		nombres_usuarios[i] = strdup(hash_iter_ver_actual(iterador));
		hash_iter_avanzar(iterador);
	}
	hash_iter_destruir(iterador);

	/* Ordenamos el arreglo alfabeticamente (menos significativo) */
	radix_sort(nombres_usuarios, hash,  n, MAX_LEN_USUARIO);
	/* Ordenamos el arreglo por cantidad de tags unicos (mas significativo) */
	counting_sort(nombres_usuarios, cant_tags, hash, n, 1, maximo, 0);

	/* Imprimimos los datos y liberamos las claves */
	hash_t *hash_usuario = hash_obtener(hash, nombres_usuarios[0]);
	size_t anterior = hash_cantidad(hash_usuario);
	printf("%zu: %s", hash_cantidad(hash_usuario), nombres_usuarios[0]);
	for (size_t i = 1; i < n; i++) {
		hash_usuario = hash_obtener(hash, nombres_usuarios[i]);
		size_t actual = hash_cantidad(hash_usuario);
		if (actual == anterior) {
			printf(", %s", nombres_usuarios[i]);
		} else {
			anterior = actual;
			printf("\n%zu: %s", hash_cantidad(hash_usuario), nombres_usuarios[i]);
		}
	}
	printf("\n");
	for (size_t i = 0; i < n; i++) {
		free(nombres_usuarios[i]);
	}
	free(nombres_usuarios);
}

/* ******************************************************************
 *                   MAIN PROCESAR USUARIOS
 * *****************************************************************/

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Parametros inválidos\n");
        return -1;
    }

	char* nombre_archivo = argv[1];
	FILE *archivo = fopen(nombre_archivo, "r");

	if (archivo == NULL) {
        printf("%s\n", "Ocurrio un error en la apertura del archivo");
        return -1;
    }

	char *linea = NULL;
	size_t capacidad = 0;
	size_t maximo = 0; // Numero mas alto de tags que tiene un usuario
	hash_t *hash = hash_crear(dato_destruir);

	while (getline(&linea, &capacidad, archivo) != EOF) {
		procesar_linea(linea, hash, &maximo);
	}
	free(linea);
	ordenar(hash, maximo);
	hash_destruir(hash);
	fclose(archivo);
	return 0;
}
