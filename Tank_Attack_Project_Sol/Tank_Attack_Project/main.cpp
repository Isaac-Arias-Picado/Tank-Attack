#include <SFML/Graphics.hpp>
#include <optional>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Graph.h"
#include "Jugador.h"
#include "Cola_Prioridad.h"
#include "Cola.h"
#include "Powerup.h"

int main() {
    srand(time(NULL));

    Graph mapa(20, 20);
    Jugador jugador1(0, "Jugador 1", &mapa);
    Jugador jugador2(1, "Jugador 2", &mapa);

    jugador1.asignarTanques();
    jugador2.asignarTanques();

    mapa.generarObstaculos(50);

    // Contar obstáculos
    int obstaculos = 0;
    int libres = 0;
    for (int i = 0; i < 400; i++) {
        if (!mapa.disponible(i)) obstaculos++;
        else libres++;
    }
    std::cout << "Obstaculos colocados: " << obstaculos - 8 << std::endl; // -8 por los tanques
    std::cout << "Nodos libres: " << libres << std::endl;
    std::cout << "Mapa conectado: " << (mapa.estaConectado() ? "SI" : "NO") << std::endl;
    // Test Jugador1 (Rojo/Amarillo - Dijkstra 80% / Aleatorio 20%) 
    std::cout << "Jugador1: 20 movimientos" << std::endl;
    Tanque* tanque1 = jugador1.getTanque(0);

    for (int mov = 0; mov < 20; mov++) {
        int origen = tanque1->getNodoActual();
        // Buscar destino aleatorio disponible
        int destino = -1;
        for (int intento = 0; intento < 1000; intento++) {
            int candidato = rand() % 400;
            if (mapa.disponible(candidato) && candidato != origen) {
                destino = candidato;
                break;
            }
        }
        if (destino == -1) { std::cout << "No hay destino disponible" << std::endl; continue; }

        std::cout << "Mov " << mov + 1 << ": " << origen << " -> " << destino << std::endl;
        tanque1->mover_tanque(destino);
        while (tanque1->enMovimiento()) tanque1->paso();
        std::cout << "  Llego a: " << tanque1->getNodoActual() << std::endl;
    }

    // Test Jugador2 (Azul/Celeste - BFS 50% / Aleatorio 50%)
    std::cout << "\nJugador2: 20 movimientos " << std::endl;
    Tanque* tanque2 = jugador2.getTanque(0);

    for (int mov = 0; mov < 20; mov++) {
        int origen = tanque2->getNodoActual();
        int destino = -1;
        for (int intento = 0; intento < 1000; intento++) {
            int candidato = rand() % 400;
            if (mapa.disponible(candidato) && candidato != origen) {
                destino = candidato;
                break;
            }
        }
        if (destino == -1) { std::cout << "No hay destino disponible" << std::endl; continue; }

        std::cout << "Mov " << mov + 1 << ": " << origen << " -> " << destino << std::endl;
        tanque2->mover_tanque(destino);
        while (tanque2->enMovimiento()) tanque2->paso();
        std::cout << "  Llego a: " << tanque2->getNodoActual() << std::endl;
    }


    return 0;
}