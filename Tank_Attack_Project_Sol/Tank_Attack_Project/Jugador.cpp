#include "Jugador.h"
#include "Graph.h"
#include "Node.h"
#include "Tanque.h"
#include "Cola.h"
#include <cstdlib>
#include <cstring>

Jugador::Jugador(int id, const char* nombre, Graph* grafo) {
    this->id = id;
    this->grafo = grafo;
    tanquesVivos = 0;
    precisionMovimiento = false;
    precisionAtaque = false;
    poderAtaque = false;
    powerupPendiente = nullptr;
    colaPowerups = new Cola();

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
        int nodoInicial;
        do {
            nodoInicial = rand() % (20 * 20);
        } while (!grafo->disponible(nodoInicial));

        char color;
        if (id == 0) {
            color = (i < 2) ? 'Z' : 'R'; // aZul, Rojo
        }
        else {
            color = (i < 2) ? 'C' : 'A'; // Celeste, Amarillo
        }

        tanques[i] = new Tanque(id, color, nodoInicial, grafo, this);
        tanquesVivos++;
        grafo->getNodo(nodoInicial)->setObjeto(tanques[i]);
    }
}

int Jugador::getTanquesVivos() {
    return tanquesVivos;
}

void Jugador::tanqueMuerto() {
    tanquesVivos--;
}

Tanque* Jugador::getTanque(int index) const {
    if (index < 0 || index >= 4) return nullptr;
    return tanques[index];
}

Cola* Jugador::getCola() {
    return colaPowerups;
}

void Jugador::setPrecisionMovimiento(bool valor) { 
    precisionMovimiento = valor; 
}
void Jugador::setPrecisionAtaque(bool valor) { 
    precisionAtaque = valor; 
}
void Jugador::setPoderAtaque(bool valor) { 
    poderAtaque = valor; 
}
bool Jugador::getPrecisionMovimiento() { 
    return precisionMovimiento; 
}
bool Jugador::getPrecisionAtaque() { 
    return precisionAtaque; 
}
bool Jugador::getPoderAtaque() { 
    return poderAtaque; 
}

void Jugador::setPowerupPendiente(Powerup* p) {
    powerupPendiente = p;
}

Powerup* Jugador::getPowerupPendiente() {
    return powerupPendiente;
}
