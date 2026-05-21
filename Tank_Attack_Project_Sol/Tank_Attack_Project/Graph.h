#pragma once
#include "Obstaculo.h"
class Node;
class Object;

class Graph {
private:
    int n; //cantidad de nodos
	int largo;
	int ancho;
    bool** matrizAdy;
    Node** nodos;

public:
    Graph(int largo, int ancho);
	bool disponible(int nodoId);
    Node* getNodo(int id);
    int getN() const { return n; }
    bool** getMatriz() const { return matrizAdy; }
    int getAncho() const { return ancho; }
    int getLargo() const { return largo; }
    void generarObstaculos(int cantidad);
    bool estaConectado();

};
