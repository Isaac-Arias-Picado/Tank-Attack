#include <SFML/Graphics.hpp>
#include <optional>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Graph.h"
#include "Jugador.h"

int main() {
    srand(time(NULL));

    Graph mapa(20, 20);
    Jugador jugador1(0, "Jugador 1", &mapa);
    Jugador jugador2(1, "Jugador 2", &mapa);

    jugador1.asignarTanques();
    jugador2.asignarTanques();

    // Contar cuantos nodos tienen tanques
    int tanquesEncontrados = 0;
    for (int i = 0; i < 400; i++) {
        if (!mapa.disponible(i)) {
            tanquesEncontrados++;
        }
    }
    std::cout << "Total de nodos ocupados con tanques: " << tanquesEncontrados << std::endl;

    Tanque* tanqueAMover = jugador1.getTanque(0);  // tomar el primer tanque del jugador 1
    int posicionActual = tanqueAMover->getNodoActual();

    // Buscar un nodo destino disponible 
    int nodoDestino = -1;
    for (int i = 0; i < 400; i++) {
        if (mapa.disponible(i) && i != posicionActual) {
            nodoDestino = i;
            break;
        }
    }

    if (nodoDestino != -1) {
        std::cout << "Moviendo tanque de nodo " << posicionActual << " a nodo " << nodoDestino << std::endl;
        tanqueAMover->mover_tanque(nodoDestino);
    }

    int nuevosTanquesEncontrados = 0;
    for (int i = 0; i < 400; i++) {
        if (!mapa.disponible(i)) {
            nuevosTanquesEncontrados++;
        }
    }
    std::cout << "Total de nodos ocupados con tanques despues del movimiento: " << nuevosTanquesEncontrados << std::endl;

    return 0;
}