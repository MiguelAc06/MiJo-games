#include <stdio.h>
#include "catalogo.h"

int main() {
    cargar_datos();
    int opcion;

    do {
        printf("\n=========================================\n");
        printf("       CATALOGO DE VIDEOJUEGOS\n");
        printf("=========================================\n");
        printf("1. Buscar un juego por el nombre\n");
        printf("2. Busqueda por categorias\n");
        printf("3. Mostrar juegos ordenados por rating (Mayor a Menor)\n");
        printf("4. Mostrar biblioteca general (Steam y Epic)\n");
        printf("5. Mostrar biblioteca de Steam\n");
        printf("6. Mostrar biblioteca de Epic\n");
        printf("7. Agregar un juego nuevo\n");
        printf("8. Borrar biblioteca completa\n");
        printf("0. Salir del programa\n");
        printf("=========================================\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion); 
        getchar(); 

        switch(opcion) {
            case 1: buscar_por_nombre(); break;
            case 2: buscar_por_categoria(); break;
            case 3: mostrar_ordenado_rating(); break;
            case 4: mostrar_biblioteca_general(); break;
            case 5: mostrar_biblioteca_plataforma("Steam"); break;
            case 6: mostrar_biblioteca_plataforma("Epic"); break;
            case 7: agregar_juego(); break;
            case 8: borrar_biblioteca(); break;
            case 0: 
                printf("Saliendo del programa y guardando datos...\n"); 
                guardar_datos();
                liberar_arbol(raiz_arbol); 
                break;
            default: printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while(opcion != 0);

    return 0;
}