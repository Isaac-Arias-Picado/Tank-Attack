#pragma once
#include "Object.h"
#include "Pathfinding.h"
#include "Graph.h"
class Tanque : public Object {
private:
    int jugador;
    char color;
    float vida;
    float vidaMaxima;
    int nodoActual;
    Graph* grafo;
    Path pathActual;

public:
    Tanque(int jugador, char color, int nodoInicial,Graph* grafo);
    const char* getTipo() const override;  
    void mover_tanque(int nuevaCasilla);
    void paso();
    bool enMovimiento() const {
        return pathActual.indiceActual < pathActual.longitud;}
    int getNodoActual();
    void recibirdanho();
    void recibirdanhototal();
};