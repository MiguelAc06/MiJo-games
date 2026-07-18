#ifndef CATALOGO_H
#define CATALOGO_H

#define MAX_JUEGOS 1000
#define MAX_STR 100

typedef struct {
    int id;
    char nombre[MAX_STR];
    char plataforma[MAX_STR]; 
    float rating;
    char categoria[MAX_STR];
} Videojuego;

typedef struct NodoArbol {
    Videojuego dato;
    struct NodoArbol *izquierdo;
    struct NodoArbol *derecho;
} NodoArbol;

extern Videojuego catalogo[MAX_JUEGOS];
extern int total_juegos;
extern const char* ARCHIVO_DB; 
extern NodoArbol *raiz_arbol;

void cargar_datos();
void guardar_datos();
void buscar_por_nombre();
void buscar_por_categoria();
void mostrar_ordenado_rating();
void mostrar_biblioteca_general();
void mostrar_biblioteca_plataforma(const char* plataforma);
void agregar_juego();
void borrar_biblioteca();

int comparar_ignorable(const char *s1, const char *s2);
int contiene_ignorable(const char *str, const char *buscar);

NodoArbol* insertar_nodo_arbol(NodoArbol *raiz, Videojuego juego);
NodoArbol* buscar_en_arbol(NodoArbol *raiz, const char *nombre);
void liberar_arbol(NodoArbol *raiz);

#endif