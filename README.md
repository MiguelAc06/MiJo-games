# MiJo-games
Este es un proyecto que consiste en una aplicación  de catálogo de videojuegos. El próposito central es modelar una base de datos ligera y eficiente en memoria dinámica para centralizar la información de bibliotecas de videojuegos repartidas en múltiples plataformas digitales como Steam, Epic Games, Playstation, Xbox y Nintendo. Cada registro dentro del sistema actuará como un nodo individual que almacenará el ID único del videojuego, su título, calificación de usuarios, género y la plataforma de origen donde fue adquirido.

Algoritmos a implementados: 
- Algoritmo de Búsqueda e Inserción en Árbol AVL 
- Algoritmo de Ordenamiento Avanzado - Merge Sort
- Algoritmo de Recorrido En-Orden (In-Order Transversal) Modificado 

Objetivos

    -Objetivo General: Desarrollar una aplicación modular en consola, denominada "MiJo Games", utilizando exclusivamente el lenguaje de programación C para centralizar bibliotecas de videojuegos en una base de datos ligera y eficiente en memoria dinámica.
Objetivos Específicos
    - Modelar cada registro de videojuego como un nodo individual que almacene su ID único, título, calificación de usuarios, género y plataforma de origen.
    - mplementar un algoritmo de búsqueda e inserción basado en un Árbol AVL para automatizar el balanceo y permitir búsquedas casi instantáneas por título o ID.
    - Aplicar algoritmos de ordenamiento avanzado (Merge Sort o Quick Sort) volcando los datos en un arreglo dinámico para generar rankings de popularidad.
    - Diseñar un recorrido En-Orden (In-Order Transversal) modificado para listar el catálogo alfabéticamente o mediante filtros selectivos por plataforma.

Justificación

    - En la actualidad, los jugadores adquieren y distribuyen sus títulos a través de múltiples plataformas digitales, como Steam y Epic Games.
    - Esta fragmentación genera un problema organizativo, ya que dificulta enormemente la centralización y la búsqueda rápida de los títulos que un usuario posee.
    - El proyecto se justifica al proporcionar una solución directa a este problema mediante una herramienta de base de datos unificada, ligera y de alto rendimiento que centraliza toda la información en un solo lugar.

Alcance

    - El proyecto consistirá estrictamente en una aplicación de consola y se desarrollará de forma modular usando únicamente el lenguaje C.
    - El sistema integrará y gestionará datos provenientes específicamente de las plataformas Steam y Epic Games.
    - La gestión de los datos se realizará enteramente en memoria dinámica, utilizando un Árbol AVL como estructura principal, arreglos dinámicos para ordenamiento.

Resultados Esperados

    - Un motor principal estable capaz de insertar y buscar videojuegos de forma eficiente, manteniendo tiempos de ejecución de $O(\log n)$ gracias a rotaciones simples o dobles.
    - Un sistema de ranking rápido que ordene los juegos por calificación de usuarios con una eficiencia temporal máxima de $O(n \log n)$.
    - Un visualizador de catálogos capaz de recorrer todos los datos en un tiempo de $O(n)$, permitiendo al usuario ver listas ordenadas alfabéticamente o filtradas por plataformas específicas.
