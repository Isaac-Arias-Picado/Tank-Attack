#include "Graph.h"
#include "Node.h"
#include "Obstaculo.h" 
#include <cstdlib>

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

void Graph::generarObstaculos(int cantidad) {
    int colocados = 0;
    int intentos = 0;

    while (colocados < cantidad && intentos < 10000) {
        intentos++;
        int id = rand() % n;
        if (!disponible(id)) continue;

        // Colocar obstáculo
        nodos[id]->setObjeto(new Obstaculo());

        // Verificar conectividad
        if (!estaConectado()) {
            // Revertir
            delete nodos[id]->getObjeto();
            nodos[id]->setObjeto(nullptr);
        }
        else {
            colocados++;
        }
    }
}

bool Graph::estaConectado() {
    // BFS desde el primer nodo disponible
    bool visitado[400] = {};
    int queue[400];
    int frente = 0, fin = 0;

    // Buscar primer nodo disponible
    int inicio = -1;
    for (int i = 0; i < n; i++) {
        if (disponible(i)) { inicio = i; break; }
    }
    if (inicio == -1) return false;

    visitado[inicio] = true;
    queue[fin++] = inicio;

    while (frente != fin) {
        int u = queue[frente++];
        for (int v = 0; v < n; v++) {
            if (!matrizAdy[u][v]) continue;
            if (!disponible(v)) continue;
            if (visitado[v]) continue;
            visitado[v] = true;
            queue[fin++] = v;
        }
    }

    // Verificar que todos los nodos disponibles fueron visitados
    for (int i = 0; i < n; i++) {
        if (disponible(i) && !visitado[i]) return false;
    }
    return true;
}