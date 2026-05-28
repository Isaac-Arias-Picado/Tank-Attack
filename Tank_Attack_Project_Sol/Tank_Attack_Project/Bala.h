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
    bool precisionAtaque;
    Graph* grafo;
    int direccionFila;
    int direccionColumna;
    Path pathActual;
    bool rebotoReciente;
    int bloquesRecorridos;

public:
    Bala(int nodoOrigen, int nodoDestino, int jugador, bool poderAtaque, Graph* grafo, bool precisionAtaque);
    void mover_bala();
    void rebotar();
    void impacto(int nodoImpacto); 
    bool estaActivo() const { return activo; }
    const char* getTipo() const override;
    Path getPath() const;
    bool getRebotoReciente() const;
    void setRebotoReciente(bool valor);
};