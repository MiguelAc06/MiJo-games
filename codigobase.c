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
    char plataforma[50]; // Steam, Epic, GOG, etc.
    
    struct GameNode *izq;
    struct GameNode *der;
    int altura;
} GameNode;

// Función auxiliar para obtener el máximo de dos enteros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Función para obtener la altura de un nodo
int obtenerAltura(GameNode *nodo) {
    if (nodo == NULL) return 0;
    return nodo->altura;
}

// Función para crear un nuevo nodo de videojuego
GameNode* crearNodo(int id, const char* titulo, float precio, float calificacion, const char* genero, const char* plataforma) {
    GameNode* nuevo = (GameNode*)malloc(sizeof(GameNode));
    if (nuevo == NULL) {
        printf("Error: No se pudo asignar memoria dinámica.\n");
        exit(1);
    }
    nuevo->id = id;
    strcpy(nuevo->titulo, titulo);
    nuevo->precio = precio;
    nuevo->calificacion = calificacion;
    strcpy(nuevo->genero, genero);
    strcpy(nuevo->plataforma, plataforma);
    
    nuevo->izq = NULL;
    nuevo->der = NULL;
    nuevo->altura = 1; // Un nuevo nodo inicialmente se inserta como hoja (altura 1)
    return nuevo;
}

// Obtener el factor de balance de un nodo
int obtenerBalance(GameNode *nodo) {
    if (nodo == NULL) return 0;
    return obtenerAltura(nodo->izq) - obtenerAltura(nodo->der);
}

// --- ROTACIONES DEL ÁRBOL AVL ---

// Rotación a la derecha (Caso Izquierda-Izquierda)
GameNode* rotarDerecha(GameNode *y) {
    GameNode *x = y->izq;
    GameNode *T2 = x->der;

    // Realizar rotación
    x->der = y;
    y->izq = T2;

    // Actualizar alturas
    y->altura = max(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;
    x->altura = max(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;

    // Retornar nueva raíz
    return x;
}

// Rotación a la izquierda (Caso Derecha-Derecha)
GameNode* rotarIzquierda(GameNode *x) {
    GameNode *y = x->der;
    GameNode *T2 = y->izq;

    // Realizar rotación
    y->izq = x;
    x->der = T2;

    // Actualizar alturas
    x->altura = max(obtenerAltura(x->izq), obtenerAltura(x->der)) + 1;
    y->altura = max(obtenerAltura(y->izq), obtenerAltura(y->der)) + 1;

    // Retornar nueva raíz
    return y;
}

// --- INSERCIÓN EN EL ÁRBOL AVL ---
// Insertamos ordenando alfabéticamente por el "titulo" del videojuego
GameNode* insertar(GameNode* nodo, int id, const char* titulo, float precio, float calificacion, const char* genero, const char* plataforma) {
    // 1. Realizar la inserción normal de un BST (Árbol Binario de Búsqueda)
    if (nodo == NULL) {
        return crearNodo(id, titulo, precio, calificacion, genero, plataforma);
    }

    // Comparamos los títulos alfabéticamente
    int comparacion = strcmp(titulo, nodo->titulo);

    if (comparacion < 0) {
        nodo->izq = insertar(nodo->izq, id, titulo, precio, calificacion, genero, plataforma);
    } else if (comparacion > 0) {
        nodo->der = insertar(nodo->der, id, titulo, precio, calificacion, genero, plataforma);
    } else {
        // Títulos idénticos no se permiten en este índice básico
        return nodo;
    }

    // 2. Actualizar la altura de este nodo ancestro
    nodo->altura = 1 + max(obtenerAltura(nodo->izq), obtenerAltura(nodo->der));

    // 3. Obtener el factor de balance de este nodo
    int balance = obtenerBalance(nodo);

    // Si el nodo se desbalanceó, se evalúan los 4 casos posibles:

    // Caso Izquierda - Izquierda
    if (balance > 1 && strcmp(titulo, nodo->izq->titulo) < 0) {
        return rotarDerecha(nodo);
    }

    // Caso Derecha - Derecha
    if (balance < -1 && strcmp(titulo, nodo->der->titulo) > 0) {
        return rotarIzquierda(nodo);
    }

    // Caso Izquierda - Derecha
    if (balance > 1 && strcmp(titulo, nodo->izq->titulo) > 0) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }

    // Caso Derecha - Izquierda
    if (balance < -1 && strcmp(titulo, nodo->der->titulo) < 0) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

// --- RECORRIDO EN-ORDEN (Para mostrar el catálogo alfabéticamente) ---
void recorridoInOrder(GameNode *raiz) {
    if (raiz != NULL) {
        recorridoInOrder(raiz->izq);
        printf("[%s] ID: %d | Titulo: %s | Genero: %s | Precio: $%.2f | Rating: %.1f/5\n", 
               raiz->plataforma, raiz->id, raiz->titulo, raiz->genero, raiz->precio, raiz->calificacion);
        recorridoInOrder(raiz->der);
    }
}

// --- LIBERAR MEMORIA (Crucial para evitar Memory Leaks) ---
void liberarArbol(GameNode *raiz) {
    if (raiz != NULL) {
        liberarArbol(raiz->izq);
        liberarArbol(raiz->der);
        free(raiz);
    }
}

// --- FUNCIÓN PRINCIPAL DE PRUEBA ---
int main() {
    GameNode *raiz = NULL;

    // Insertar algunos videojuegos de prueba en diferentes plataformas
    raiz = insertar(raiz, 101, "The Witcher 3", 39.99, 4.9, "RPG", "GOG");
    raiz = insertar(raiz, 102, "Cyberpunk 2077", 59.99, 4.2, "RPG", "Steam");
    raiz = insertar(raiz, 103, "Hades", 24.99, 4.8, "Roguelike", "Epic Games");
    raiz = insertar(raiz, 104, "Alan Wake 2", 49.99, 4.6, "Terror", "Epic Games");
    raiz = insertar(raiz, 105, "Portal 2", 9.99, 5.0, "Puzzle", "Steam");

    printf("--- CATÁLOGO DE VIDEOJUEGOS (Ordenado Alfabéticamente - AVL) ---\n\n");
    recorridoInOrder(raiz);

    // Liberar memoria antes de salir
    liberarArbol(raiz);
    raiz = NULL;

    return 0;
}