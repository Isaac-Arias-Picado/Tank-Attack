#include "Graph.h"
#include "Node.h"

Graph::Graph(int largo, int ancho) {
    this->n = largo * ancho; // total de nodos
    this->largo = largo;     
    this->ancho = ancho;     

	// Crear matriz de adyacencia n x n y listado de nodos
    nodos = new Node * [n];
    matrizAdy = new bool* [n];
    for (int i = 0; i < n; i++) {
        nodos[i] = new Node(i);
        matrizAdy[i] = new bool[n];
        for (int j = 0; j < n; j++) {
            matrizAdy[i][j] = false;
        }
    }

	//conectar nodos adyacentes 
    for (int fila = 0; fila < largo; fila++) {
        for (int columna = 0; columna < ancho; columna++) {
            int actual = fila * ancho + columna;

            // Arriba
            if (fila > 0) {
                int vecino = (fila - 1) * ancho + columna;
                matrizAdy[actual][vecino] = true;
                matrizAdy[vecino][actual] = true;
            }

            // ABAJO
            if (fila < largo - 1) {
                int vecino = (fila + 1) * ancho + columna;
                matrizAdy[actual][vecino] = true;
                matrizAdy[vecino][actual] = true;
            }

            // IZQUIERDA
            if (columna > 0) {
                int vecino = fila * ancho + (columna - 1);
                matrizAdy[actual][vecino] = true;
                matrizAdy[vecino][actual] = true;
            }

            // DERECHA
            if (columna < ancho - 1) {
                int vecino = fila * ancho + (columna + 1);
                matrizAdy[actual][vecino] = true;
                matrizAdy[vecino][actual] = true;
            }
        }
    }
}

bool Graph::disponible(int nodoId) {
    return nodos[nodoId]->getObjeto() == nullptr; // disponible si no hay objeto
}

Node* Graph::getNodo(int id) {
    if (id < 0 || id >= n) return nullptr;
    return nodos[id];
}