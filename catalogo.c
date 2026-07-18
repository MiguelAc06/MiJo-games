#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "catalogo.h"

Videojuego catalogo[MAX_JUEGOS];
int total_juegos = 0;
const char* ARCHIVO_DB = "videojuegos.txt"; 
NodoArbol *raiz_arbol = NULL;

void merge(Videojuego arr[], int izquierda, int medio, int derecha);
void merge_sort_recursivo(Videojuego arr[], int izquierda, int derecha);

void cargar_datos() {
    FILE *file = fopen(ARCHIVO_DB, "r");
    if (file == NULL) {
        printf("No se encontró el archivo %s. Se creará uno nuevo al guardar.\n", ARCHIVO_DB);
        return;
    }
    
    total_juegos = 0;
    liberar_arbol(raiz_arbol);
    raiz_arbol = NULL;

    while (fscanf(file, "%d;%[^;];%[^;];%f;%[^\n]\n", 
                  &catalogo[total_juegos].id, 
                  catalogo[total_juegos].nombre, 
                  catalogo[total_juegos].plataforma, 
                  &catalogo[total_juegos].rating, 
                  catalogo[total_juegos].categoria) == 5) {
        
        raiz_arbol = insertar_nodo_arbol(raiz_arbol, catalogo[total_juegos]);
        total_juegos++;
        if (total_juegos >= MAX_JUEGOS) break;
    }
    fclose(file);
}

void guardar_datos() {
    FILE *file = fopen(ARCHIVO_DB, "w");
    if (file == NULL) {
        printf("Error al abrir el archivo para guardar.\n");
        return;
    }
    for (int i = 0; i < total_juegos; i++) {
        fprintf(file, "%d;%s;%s;%.2f;%s\n", 
                catalogo[i].id, catalogo[i].nombre, 
                catalogo[i].plataforma, catalogo[i].rating, 
                catalogo[i].categoria);
    }
    fclose(file);
}

void buscar_por_nombre() {
    char buscador[MAX_STR];
    printf("Ingrese el nombre del juego a buscar: ");
    fgets(buscador, MAX_STR, stdin);
    buscador[strcspn(buscador, "\n")] = 0; 

    printf("\nResultados de búsqueda indexada (ABB):\n");
    
    NodoArbol *resultado = buscar_en_arbol(raiz_arbol, buscador);
    
    if(resultado != NULL) {
        Videojuego j = resultado->dato;
        printf("[%d] %s | Plataforma: %s | Rating: %.1f | Categoría: %s\n", 
               j.id, j.nombre, j.plataforma, j.rating, j.categoria);
    } else {
        int encontrados = 0;
        for(int i = 0; i < total_juegos; i++) {
            if(contiene_ignorable(catalogo[i].nombre, buscador)) {
                printf("[%d] %s | Plataforma: %s | Rating: %.1f | Categoría: %s\n", 
                       catalogo[i].id, catalogo[i].nombre, catalogo[i].plataforma, catalogo[i].rating, catalogo[i].categoria);
                encontrados++;
            }
        }
        if(encontrados == 0) printf("No se encontraron juegos con ese nombre.\n");
    }
}

void buscar_por_categoria() {
    char categorias_unicas[MAX_JUEGOS][MAX_STR];
    int total_cat = 0;

    for(int i = 0; i < total_juegos; i++) {
        int repetida = 0;
        for(int j = 0; j < total_cat; j++) {
            if(comparar_ignorable(catalogo[i].categoria, categorias_unicas[j]) == 0) {
                repetida = 1;
                break;
            }
        }
        if(!repetida) {
            strcpy(categorias_unicas[total_cat], catalogo[i].categoria);
            total_cat++;
        }
    }

    if(total_cat == 0) {
        printf("No hay categorías registradas.\n");
        return;
    }

    printf("\n--- CATEGORÍAS REGISTRADAS ---\n");
    for(int i = 0; i < total_cat; i++) {
        printf("%d- %s\n", i + 1, categorias_unicas[i]);
    }
    printf("0- Volver al menú principal\n");
    
    int opc_cat;
    printf("Seleccione una categoría: ");
    scanf("%d", &opc_cat);
    getchar();

    if(opc_cat > 0 && opc_cat <= total_cat) {
        printf("\nJuegos en la categoría '%s':\n", categorias_unicas[opc_cat - 1]);
        for(int i = 0; i < total_juegos; i++) {
            if(comparar_ignorable(catalogo[i].categoria, categorias_unicas[opc_cat - 1]) == 0) {
                printf("- %s (Rating: %.1f) [%s]\n", catalogo[i].nombre, catalogo[i].rating, catalogo[i].plataforma);
            }
        }
    }
}

void mostrar_ordenado_rating() {
    if (total_juegos == 0) {
        printf("\nNo hay juegos en el catálogo para ordenar.\n");
        return;
    }

    Videojuego temporal[MAX_JUEGOS];
    memcpy(temporal, catalogo, sizeof(Videojuego) * total_juegos);

    merge_sort_recursivo(temporal, 0, total_juegos - 1);

    printf("\n--- JUEGOS ORDENADOS POR RATING CON MERGE SORT (MAYOR A MENOR) ---\n");
    for(int i = 0; i < total_juegos; i++) {
        printf("Rating: %.1f | %s [%s]\n", temporal[i].rating, temporal[i].nombre, temporal[i].plataforma);
    }
}

void mostrar_biblioteca_general() {
    printf("\n--- BIBLIOTECA GENERAL ---\n");
    for(int i = 0; i < total_juegos; i++) {
        printf("ID: %d | %s | Plataforma: %s | Rating: %.1f | Cat: %s\n", 
               catalogo[i].id, catalogo[i].nombre, catalogo[i].plataforma, catalogo[i].rating, catalogo[i].categoria);
    }
}

void mostrar_biblioteca_plataforma(const char* plataforma) {
    printf("\n--- BIBLIOTECA DE %s ---\n", plataforma);
    int cont = 0;
    for(int i = 0; i < total_juegos; i++) {
        if(comparar_ignorable(catalogo[i].plataforma, plataforma) == 0) {
            printf("- %s (Rating: %.1f) | Cat: %s\n", catalogo[i].nombre, catalogo[i].rating, catalogo[i].categoria);
            cont++;
        }
    }
    if(cont == 0) printf("No hay juegos registrados en %s.\n", plataforma);
}

void agregar_juego() {
    if(total_juegos >= MAX_JUEGOS) {
        printf("Catálogo lleno. No se pueden agregar más juegos.\n");
        return;
    }

    Videojuego nuevo;
    printf("Ingrese el nombre del juego: ");
    fgets(nuevo.nombre, MAX_STR, stdin);
    nuevo.nombre[strcspn(nuevo.nombre, "\n")] = 0;

    int indice_existente = -1;
    for(int i = 0; i < total_juegos; i++) {
        if(comparar_ignorable(catalogo[i].nombre, nuevo.nombre) == 0) {
            indice_existente = i;
            break;
        }
    }

    if(indice_existente != -1) {
        printf("\n[!] El juego ya existe en la base de datos (ID: %d). Se heredarán sus datos.\n", catalogo[indice_existente].id);
        
        nuevo.id = catalogo[indice_existente].id;
        nuevo.rating = catalogo[indice_existente].rating;
        strcpy(nuevo.categoria, catalogo[indice_existente].categoria);

        printf("Ingrese la plataforma (Steam/Epic): ");
        fgets(nuevo.plataforma, MAX_STR, stdin);
        nuevo.plataforma[strcspn(nuevo.plataforma, "\n")] = 0;

        catalogo[total_juegos] = nuevo;
        raiz_arbol = insertar_nodo_arbol(raiz_arbol, nuevo);
        total_juegos++;
        printf("¡Nueva plataforma agregada con éxito al juego existente!\n");
    } 
    else {
        printf("Ingrese la plataforma (Steam/Epic): ");
        fgets(nuevo.plataforma, MAX_STR, stdin);
        nuevo.plataforma[strcspn(nuevo.plataforma, "\n")] = 0;

        printf("Ingrese el rating (0.0 a 5.0 o 10.0): ");
        scanf("%f", &nuevo.rating);
        getchar();

        printf("Ingrese la categoría: ");
        fgets(nuevo.categoria, MAX_STR, stdin);
        nuevo.categoria[strcspn(nuevo.categoria, "\n")] = 0;

        int id_maximo = 0;
        for(int i = 0; i < total_juegos; i++) {
            if(catalogo[i].id > id_maximo) id_maximo = catalogo[i].id;
        }
        nuevo.id = id_maximo + 1;

        catalogo[total_juegos] = nuevo;
        raiz_arbol = insertar_nodo_arbol(raiz_arbol, nuevo);
        total_juegos++;
        printf("¡Juego agregado exitosamente con ID %d! Se guardará de forma permanente al salir.\n", nuevo.id);
    }
}

void borrar_biblioteca() {
    char confirmacion;
    printf("\n[¡ADVERTENCIA!] ¿Está seguro de que desea BORRAR COMPLETAMENTE la biblioteca? (S/N): ");
    scanf(" %c", &confirmacion);
    getchar();

    if (confirmacion == 'S' || confirmacion == 's') {
        total_juegos = 0;
        liberar_arbol(raiz_arbol);
        raiz_arbol = NULL;
        
        FILE *file = fopen(ARCHIVO_DB, "w");
        if (file != NULL) {
            fclose(file); 
        }
        printf("Biblioteca vaciada y archivo de texto limpiado con éxito.\n");
    } else {
        printf("Operación cancelada. Los juegos están a salvo.\n");
    }
}

int comparar_ignorable(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
        }
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

int contiene_ignorable(const char *str, const char *buscar) {
    if (*buscar == '\0') return 1;
    for (; *str != '\0'; str++) {
        if (tolower((unsigned char)*str) == tolower((unsigned char)*buscar)) {
            const char *h = str;
            const char *n = buscar;
            while (*h != '\0' && *n != '\0' && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
                h++;
                n++;
            }
            if (*n == '\0') return 1;
        }
    }
    return 0;
}

NodoArbol* insertar_nodo_arbol(NodoArbol *raiz, Videojuego juego) {
    if (raiz == NULL) {
        NodoArbol *nuevo_nodo = (NodoArbol*)malloc(sizeof(NodoArbol));
        nuevo_nodo->dato = juego;
        nuevo_nodo->izquierdo = NULL;
        nuevo_nodo->derecho = NULL;
        return nuevo_nodo;
    }
    if (comparar_ignorable(juego.nombre, raiz->dato.nombre) < 0) {
        raiz->izquierdo = insertar_nodo_arbol(raiz->izquierdo, juego);
    } else {
        raiz->derecho = insertar_nodo_arbol(raiz->derecho, juego);
    }
    return raiz;
}

NodoArbol* buscar_en_arbol(NodoArbol *raiz, const char *nombre) {
    if (raiz == NULL || comparar_ignorable(raiz->dato.nombre, nombre) == 0) {
        return raiz;
    }
    if (comparar_ignorable(nombre, raiz->dato.nombre) < 0) {
        return buscar_en_arbol(raiz->izquierdo, nombre);
    }
    return buscar_en_arbol(raiz->derecho, nombre);
}

void liberar_arbol(NodoArbol *raiz) {
    if (raiz != NULL) {
        liberar_arbol(raiz->izquierdo);
        liberar_arbol(raiz->derecho);
        free(raiz);
    }
}

void merge(Videojuego arr[], int izquierda, int medio, int derecha) {
    int i, j, k;
    int n1 = medio - izquierda + 1;
    int n2 = derecha - medio;

    Videojuego *L = (Videojuego*)malloc(n1 * sizeof(Videojuego));
    Videojuego *R = (Videojuego*)malloc(n2 * sizeof(Videojuego));

    for (i = 0; i < n1; i++) L[i] = arr[izquierda + i];
    for (j = 0; j < n2; j++) R[j] = arr[medio + 1 + j];

    i = 0; 
    j = 0; 
    k = izquierda; 

    while (i < n1 && j < n2) {
        if (L[i].rating >= R[j].rating) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void merge_sort_recursivo(Videojuego arr[], int izquierda, int derecha) {
    if (izquierda < derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;

        merge_sort_recursivo(arr, izquierda, medio);
        merge_sort_recursivo(arr, medio + 1, derecha);

        merge(arr, izquierda, medio, derecha);
    }
}