#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Graph.h"
#include "Jugador.h"
#include "Renderer.h"
#include "SistemaTurnos.h"
#include "Tanque.h"

int main() {
    srand(time(NULL));

    Graph mapa(20, 20);
    Jugador jugador1(0, "Jugador 1", &mapa);
    Jugador jugador2(1, "Jugador 2", &mapa);

    jugador1.asignarTanques();
    jugador2.asignarTanques();
    mapa.generarObstaculos(50);

    SistemaTurnos turnos(&jugador1, &jugador2);
    Renderer renderer(&mapa, &jugador1, &jugador2);
    if (!renderer.init()) return -1;

    Tanque* tanqueSeleccionado = nullptr;
    bool esperandoDestino = false;

    while (renderer.isOpen()) {
        // Verificar victoria
        Jugador* ganador = turnos.verificarVictoria();
        if (ganador != nullptr) {
            std::cout << "Gano: " << ganador->getNombre() << std::endl;
            break;
        }

        // Actualizar HUD con el jugador activo
        renderer.updateTurnDisplay(turnos.getJugadorActivo());

        // Avanzar movimiento
        if (tanqueSeleccionado != nullptr && !esperandoDestino) {
            if (tanqueSeleccionado->enMovimiento()) {
                tanqueSeleccionado->paso();
            }
            else {
                turnos.completarTurno();
                turnos.siguienteTurno();
                tanqueSeleccionado = nullptr;
                esperandoDestino = false;
            }
        }

        // Eventos
        while (const std::optional event = renderer.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                renderer.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                auto* mb = event->getIf<sf::Event::MouseButtonPressed>();
                int mouseX = mb->position.x;
                int mouseY = mb->position.y;
                int col = mouseX / 64;
                int row = mouseY / 64;
                int nodoClick = row * 20 + col;

                std::cout << "Click en pixel (" << mouseX << "," << mouseY << ") -> nodo " << nodoClick << std::endl;

                if (mb->button == sf::Mouse::Button::Left) {
                    Jugador* activo = turnos.getJugadorActivo();

                    if (tanqueSeleccionado == nullptr) {
                        // Primer click: seleccionar tanque
                        for (int i = 0; i < 4; i++) {
                            Tanque* t = activo->getTanque(i);
                            if (t != nullptr && t->getNodoActual() == nodoClick) {
                                tanqueSeleccionado = t;
                                esperandoDestino = true;
                                std::cout << "Tanque seleccionado en nodo " << nodoClick << std::endl;
                                break;
                            }
                        }
                        if (tanqueSeleccionado == nullptr)
                            std::cout << "Ningun tanque en ese nodo" << std::endl;

                    }
                    else if (esperandoDestino) {
                        // Segundo click: destino
                        if (nodoClick >= 0 && nodoClick < 400 && mapa.disponible(nodoClick)) {
                            tanqueSeleccionado->mover_tanque(nodoClick);
                            esperandoDestino = false;
                            std::cout << "Moviendo a nodo " << nodoClick << std::endl;
                        }
                        else {
                            std::cout << "Destino invalido o bloqueado" << std::endl;
                        }
                    }
                }
            }
        }

        renderer.render();
    }

    return 0;
}