#include "Jugador.h"
#include "Graph.h"
#include "Node.h"
#include <cstdlib>
#include <cstring>

Jugador::Jugador(int id, const char* nombre, Graph* grafo) {
    this->id = id;
    this->grafo = grafo;
    this->tanquesVivos = 0;

    // Copiar nombre
    int i = 0;
    while (nombre[i] != '\0' && i < 49) {
        this->nombre[i] = nombre[i];
        i++;
    }
    this->nombre[i] = '\0';

    // Crear arreglo de 4 tanques
    this->tanques = new Tanque * [4];
    for (int i = 0; i < 4; i++) {
        this->tanques[i] = nullptr;
    }   
}

void Jugador::asignarTanques() {
    for (int i = 0; i < 4; i++) {
        int color;
        int nodoInicial;
        do {
            nodoInicial = rand() % (20 * 20);
        } while (!grafo->disponible(nodoInicial));

        if (id == 0) {
            if (i < 2) {
                color = 0;  // azul
            }
            else {
                color = 3;  // rojo
            }
        }
        else {
            if (i < 2) {
                color = 1;  // celeste
            }
            else {
                color = 2;  // amarillo
            }
        }

        tanques[i] = new Tanque(id, color, nodoInicial, grafo);
        tanquesVivos++;
        Node* nodo = grafo->getNodo(nodoInicial);
        nodo->setObjeto(tanques[i]);
    }
}

Tanque* Jugador::getTanque(int index) const {
    if (index < 0 || index >= 4) return nullptr;
    return tanques[index];
}