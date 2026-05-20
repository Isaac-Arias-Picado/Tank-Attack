#pragma once
#include "Object.h"
#include "Pathfinding.h"
#include "Graph.h"
#include "Node.h"

class Bala : public Object {
private:
    int nodoActual;
    int nodoDestino;
    int jugador;
    bool poderAtaque;
    int rebotes;
    bool activo;
    Graph* grafo;
    int direccionFila;
    int direccionColumna;
    Path pathActual;

public:
    Bala(int nodoOrigen, int nodoDestino, int jugador, bool poderAtaque, Graph* grafo);
    void mover_bala();
    void rebotar();
    void impacto();
    bool estaActivo() const { return activo; }
    const char* getTipo() const override;
};