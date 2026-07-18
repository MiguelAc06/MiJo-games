#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura del nodo del videojuego (Nodo AVL)
typedef struct GameNode {
    int id;
    char titulo[100];
    float precio;
    float calificacion;
    char genero[50];
    char plataforma[50]; 
    
    struct GameNode *izq;
    struct GameNode *der;
    int altura;
} GameNode;

// Variables globales para control de IDs autoincrementables y conteo
int proximo_id = 106; 
int total_juegos = 0;

// --- FUNCIONES AUXILIARES AVL ---
int max(int a, int b) { return (a > b) ? a : b; }

int obtenerAltura(GameNode *nodo) {
    if (nodo == NULL) return 0;
    return nodo->altura;
}

int obtenerBalance(GameNode *nodo) {
    if (nodo == NULL) return 0;
    return obtenerAltura(nodo->izq) - obtenerAltura(nodo->der);
}

GameNode* crearNodo(int id, const char* titulo, float precio, float calificacion, const char* genero, const char* plataforma) {
    GameNode* nuevo = (GameNode*)malloc(sizeof(GameNode));
    if (nuevo == NULL) {
        printf("Error de asignacion de memoria.\n");
        exit(1);
    }
    nuevo->id = id;
    strcpy(nuevo->titulo, titulo);
    nuevo->precio = precio;
    nuevo->calificacion = calificacion;
    strcpy(nuevo->genero, genero);
    strcpy(nuevo->plataforma, plataforma);
    nuevo->izq = nuevo->der = NULL;
    nuevo->altura = 1;
    total_juegos++;
    return nuevo;
}

// --- ROTACIONES AVL ---
GameNode* rotarDerecha(GameNode *y) {
    GameNode *x = y->izq;
    GameNode *T2 = x->der;
    x->der = y;
    y->izq = T2;
    y->altura = max(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;
    x->altura = max(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;
    return x;
}

GameNode* rotarIzquierda(GameNode *x) {
    GameNode *y = x->der;
    GameNode *T2 = y->izq;
    y->izq = x;
    x->der = T2;
    x->altura = max(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;
    y->altura = max(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;
    return y;
}

// --- INSERCIÓN AVL ---
GameNode* insertar(GameNode* nodo, int id, const char* titulo, float precio, float calificacion, const char* genero, const char* plataforma) {
    if (nodo == NULL) return crearNodo(id, titulo, precio, calificacion, genero, plataforma);

    int comparacion = strcmp(titulo, nodo->titulo);
    if (comparacion < 0)
        nodo->izq = insertar(nodo->izq, id, titulo, precio, calificacion, genero, plataforma);
    else if (comparacion > 0)
        nodo->der = insertar(nodo->der, id, titulo, precio, calificacion, genero, plataforma);
    else {
        printf("\n[!] El juego '%s' ya existe en el catalogo.\n", titulo);
        return nodo; // No duplicados
    }

    nodo->altura = 1 + max(obtenerAltura(nodo->izq), obtenerAltura(nodo->der));
    int balance = obtenerBalance(nodo);

    if (balance > 1 && strcmp(titulo, nodo->izq->titulo) < 0) return rotarDerecha(nodo);
    if (balance < -1 && strcmp(titulo, nodo->der->titulo) > 0) return rotarIzquierda(nodo);
    if (balance > 1 && strcmp(titulo, nodo->izq->titulo) > 0) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }
    if (balance < -1 && strcmp(titulo, nodo->der->titulo) < 0) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }
    return nodo;
}

// --- REQUERIMIENTO 1: BUSCAR JUEGO POR NOMBRE ($O(\log n)$) ---
GameNode* buscarPorNombre(GameNode* raiz, const char* titulo) {
    if (raiz == NULL) return NULL;
    int comp = strcmp(titulo, raiz->titulo);
    if (comp == 0) return raiz;
    if (comp < 0) return buscarPorNombre(raiz->izq, titulo);
    return buscarPorNombre(raiz->der, titulo);
}

// --- REQUERIMIENTO 4, 5 Y 6: MOSTRAR BIBLIOTECAS FILTRADAS ($O(n)$) ---
// Si plataforma_filtro es "General", muestra todos. Si no, filtra por la cadena recibida.
void mostrarBiblioteca(GameNode* raiz, const char* plataforma_filtro) {
    if (raiz != NULL) {
        mostrarBiblioteca(raiz->izq, plataforma_filtro);
        if (strcmp(plataforma_filtro, "General") == 0 || strcasecmp(raiz->plataforma, plataforma_filtro) == 0) {
            printf("ID: %-4d | %-20s | %-12s | %-12s | Rating: %.1f/5\n", 
                   raiz->id, raiz->titulo, raiz->genero, raiz->plataforma, raiz->calificacion);
        }
        mostrarBiblioteca(raiz->der, plataforma_filtro);
    }
}

// --- REQUERIMIENTO 2: BUSQUEDA POR CATEGORÍAS ---
void filtrarPorCategoria(GameNode* raiz, const char* categoria) {
    if (raiz != NULL) {
        filtrarPorCategoria(raiz->izq, categoria);
        if (strcasecmp(raiz->genero, categoria) == 0) {
            printf("ID: %-4d | %-20s | %-12s | Rating: %.1f/5\n", raiz->id, raiz->titulo, raiz->plataforma, raiz->calificacion);
        }
        filtrarPorCategoria(raiz->der, categoria);
    }
}

// --- REQUERIMIENTO 3: RECOPILACIÓN Y MERGE SORT (ORDENAR POR RATING DE MAYOR A MENOR) ---
// Pasa los nodos del árbol a un arreglo plano secuencial
void volcarArreglo(GameNode* raiz, GameNode** arreglo, int* indice) {
    if (raiz != NULL) {
        volcarArreglo(raiz->izq, arreglo, indice);
        arreglo[*indice] = raiz;
        (*indice)++;
        volcarArreglo(raiz->der, arreglo, indice);
    }
}

// Fusión del Merge Sort (Descendente por calificación)
void merge(GameNode** arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    GameNode** L = (GameNode**)malloc(n1 * sizeof(GameNode*));
    GameNode** R = (GameNode**)malloc(n2 * sizeof(GameNode*));

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i]->calificacion >= R[j]->calificacion) { // >= para descendente
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
    free(L); free(R);
}

void mergeSort(GameNode** arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void ordenarPorRating(GameNode* raiz) {
    if (raiz == NULL) {
        printf("El catalogo esta vacio.\n");
        return;
    }
    GameNode** arr = (GameNode**)malloc(total_juegos * sizeof(GameNode*));
    int idx = 0;
    volcarArreglo(raiz, arr, &idx);

    mergeSort(arr, 0, total_juegos - 1);

    printf("\n--- JUEGOS ORDENADOS POR RATING (MAYOR A MENOR) ---\n");
    for (int i = 0; i < total_juegos; i++) {
        printf("Rating: %.1f/5 | %-20s | %-12s\n", arr[i]->calificacion, arr[i]->titulo, arr[i]->plataforma);
    }
    free(arr);
}

// --- LIMPIEZA DE MEMORIA ---
void liberarArbol(GameNode *raiz) {
    if (raiz != NULL) {
        liberarArbol(raiz->izq);
        liberarArbol(raiz->der);
        free(raiz);
    }
}

// --- FUNCIÓN PRINCIPAL INTERACTIVA ---
int main() {
    GameNode *raiz = NULL;

    // Precarga de datos iniciales
    raiz = insertar(raiz, 101, "The Witcher 3", 39.99, 4.9, "RPG", "GOG");
    raiz = insertar(raiz, 102, "Cyberpunk 2077", 59.99, 4.2, "RPG", "Steam");
    raiz = insertar(raiz, 103, "Hades", 24.99, 4.8, "Roguelike", "Epic");
    raiz = insertar(raiz, 104, "Alan Wake 2", 49.99, 4.6, "Terror", "Epic");
    raiz = insertar(raiz, 105, "Portal 2", 9.99, 5.0, "Puzzle", "Steam");

    int opcion;
    char buffer_busqueda[100];

    do {
        printf("\n=========================================\n");
        printf("       CATALOGO DE VIDEOJUEGOS (AVL)     \n");
        printf("=========================================\n");
        printf("1- Buscar un juego por el nombre\n");
        printf("2- Busqueda por categorias\n");
        printf("3- Mostrar juegos ordenados por Rating (Mayor a Menor)\n");
        printf("4- Mostrar biblioteca General (Steam y Epic/GOG)\n");
        printf("5- Mostrar biblioteca de Steam\n");
        printf("6- Mostrar biblioteca de Epic\n");
        printf("7- Agregar un juego nuevo\n");
        printf("0- Salir del programa\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar(); // Limpiar el salto de línea del buffer

        switch(opcion) {
            case 1:
                printf("\nIngrese el nombre del juego a buscar: ");
                fgets(buffer_busqueda, sizeof(buffer_busqueda), stdin);
                buffer_busqueda[strcspn(buffer_busqueda, "\n")] = 0; // Remover el enter
                
                GameNode* encontrado = buscarPorNombre(raiz, buffer_busqueda);
                if (encontrado != NULL) {
                    printf("\n[+] ¡Juego Encontrado!\n");
                    printf("ID: %d | Titulo: %s | Plataforma: %s | Genero: %s | Rating: %.1f\n", 
                           encontrado->id, encontrado->titulo, encontrado->plataforma, encontrado->genero, encontrado->calificacion);
                } else {
                    printf("\n[-] El juego '%s' no se encuentra registrado.\n", buffer_busqueda);
                }
                break;

            case 2:
                printf("\n--- CATEGORIAS DISPONIBLES ---\n");
                printf("1. RPG\n2. Roguelike\n3. Terror\n4. Puzzle\n");
                printf("Seleccione o escriba la categoria: ");
                fgets(buffer_busqueda, sizeof(buffer_busqueda), stdin);
                buffer_busqueda[strcspn(buffer_busqueda, "\n")] = 0;
                
                printf("\n--- JUEGOS EN LA CATEGORIA: %s ---\n", buffer_busqueda);
                filtrarPorCategoria(raiz, buffer_busqueda);
                break;

            case 3:
                ordenarPorRating(raiz);
                break;

            case 4:
                printf("\n--- BIBLIOTECA GENERAL ---\n");
                mostrarBiblioteca(raiz, "General");
                break;

            case 5:
                printf("\n--- BIBLIOTECA DE STEAM ---\n");
                mostrarBiblioteca(raiz, "Steam");
                break;

            case 6:
                printf("\n--- BIBLIOTECA DE EPIC ---\n");
                mostrarBiblioteca(raiz, "Epic");
                break;

            case 7: {
                char n_titulo[100], n_plataforma[50], n_categoria[50];
                float n_rating, n_precio;

                printf("\nIngrese el nombre del juego: ");
                fgets(n_titulo, sizeof(n_titulo), stdin);
                n_titulo[strcspn(n_titulo, "\n")] = 0;

                // Validar si ya existe antes de pedir el resto de datos
                if (buscarPorNombre(raiz, n_titulo) != NULL) {
                    printf("\n[!] Error: El juego ya existe en la base de datos.\n");
                    break;
                }

                printf("Ingrese la plataforma (Steam, Epic, GOG, etc.): ");
                fgets(n_plataforma, sizeof(n_plataforma), stdin);
                n_plataforma[strcspn(n_plataforma, "\n")] = 0;

                printf("Ingrese la categoria/genero: ");
                fgets(n_categoria, sizeof(n_categoria), stdin);
                n_categoria[strcspn(n_categoria, "\n")] = 0;

                printf("Ingrese el rating (0.0 a 5.0): ");
                scanf("%f", &n_rating);

                printf("Ingrese el precio: $");
                scanf("%f", &n_precio);

                raiz = insertar(raiz, proximo_id++, n_titulo, n_precio, n_rating, n_categoria, n_plataforma);
                printf("\n[+] Juego guardado exitosamente con ID: %d\n", proximo_id - 1);
                break;
            }

            case 0:
                printf("\nSaliendo del sistema de gestion de juegos...\n");
                break;

            default:
                printf("\n[!] Opcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 0);

    liberarArbol(raiz);
    return 0;
}