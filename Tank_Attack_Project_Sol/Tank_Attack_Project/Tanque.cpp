#include "Tanque.h"
#include "Graph.h"    
#include "Node.h" 

Tanque::Tanque(int jugador, int color, int nodo, Graph * grafo){
	this->jugador = jugador;
	this->color = color;
	this->nodoActual = nodo;
	this->vidaMaxima = 100;
	this->vida = vidaMaxima;
	this->grafo = grafo;
}

const char* Tanque::getTipo() const {
	return "Tanque";
}

void Tanque::mover_tanque(int nuevaCasilla) {
    if (!grafo->disponible(nuevaCasilla)) return;
    grafo->getNodo(nodoActual)->setObjeto(nullptr);
    nodoActual = nuevaCasilla;
    grafo->getNodo(nuevaCasilla)->setObjeto(this);
}

int Tanque::getNodoActual(){
	return nodoActual;
}