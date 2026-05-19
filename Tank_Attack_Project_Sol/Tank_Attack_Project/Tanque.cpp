#include "Tanque.h"
#include "Graph.h"    
#include "Node.h" 
#include <cstdlib>
#include "Jugador.h"
#include <iostream>

Tanque::Tanque(int jugador, char color, int nodo, Graph * grafo, Jugador* objjugador){
	this->jugador = jugador;
	this->color = color;
	this->nodoActual = nodo;
	this->vidaMaxima = 100;
	this->vida = vidaMaxima;
	this->grafo = grafo;
    this->objjugador = objjugador;
}

const char* Tanque::getTipo() const {
	return "Tanque";
}

void Tanque::mover_tanque(int nuevaCasilla) {
    if (!grafo->disponible(nuevaCasilla)) return;

    int roll = rand() % 100;

    if (color == 'A' || color == 'C') {        // Azul/Celeste: 50/50
        if (roll < 50) {
            pathActual = bfs(grafo, nodoActual, nuevaCasilla);
            std::cout << "Tanque " << color << " usando BFS" << std::endl;
        }
        else {
            pathActual = movimientoAleatorio(grafo, nodoActual, nuevaCasilla);
            std::cout << "Tanque " << color << " usando Movimiento Aleatorio" << std::endl;
        }
    }
    else {                                    // Amarillo/Rojo: 80/20
        if (roll < 80) {
            pathActual = dijkstra(grafo, nodoActual, nuevaCasilla);
            std::cout << "Tanque " << color << " usando Dijkstra" << std::endl;
        }
        else {
            pathActual = movimientoAleatorio(grafo, nodoActual, nuevaCasilla);
            std::cout << "Tanque " << color << " usando Movimiento Aleatorio" << std::endl;
        }
    }
}

void Tanque::paso() {
    if (!enMovimiento()) return;

    int siguiente = pathActual.nodos[pathActual.indiceActual];

    if (!grafo->disponible(siguiente)) return; 

    grafo->getNodo(nodoActual)->setObjeto(nullptr);
    nodoActual = siguiente;
    grafo->getNodo(nodoActual)->setObjeto(this);

    pathActual.indiceActual++;
}
int Tanque::getNodoActual(){
	return nodoActual;
}

void Tanque::recibirdanho() {
    if (color == 'C' || color == 'A') {
        vida -= 25;
    }
    else {
        vida -= 50;
    }
    if (vida <= 0) {
        activo = false;
        if (grafo->getNodo(nodoActual) != nullptr)
            grafo->getNodo(nodoActual)->setObjeto(nullptr);
        objjugador->tanqueMuerto();
    }
}

void Tanque::recibirdanhototal() {
    vida = 0;
    activo = false;
    if (grafo->getNodo(nodoActual) != nullptr)
        grafo->getNodo(nodoActual)->setObjeto(nullptr);
    objjugador->tanqueMuerto();
}