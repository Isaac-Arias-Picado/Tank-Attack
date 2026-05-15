// Pathfinding.h
#pragma once
#include "Graph.h"

struct Path {
    int nodos[400];
    int longitud;
    int indiceActual;

    Path() : longitud(0), indiceActual(0) {}
};

Path dijkstra(Graph* grafo, int origen, int destino);
Path bfs(Graph* grafo, int origen, int destino);
Path movimientoAleatorio(Graph* grafo, int origen, int destino, int radio = 3);
int heuristica(int origen, int destino, int ancho);