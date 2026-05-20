#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Graph.h"
#include "Jugador.h"
#include "Renderer.h"
#include "SistemaTurnos.h"
#include "Tanque.h"
#include "Bala.h"
#include "Cola.h"
#include "Powerup.h"

const int MAX_BALAS = 20;

int main() {
    srand(time(NULL));

    Graph mapa(20, 20);
    Jugador jugador1(0, "Jugador 1", &mapa);
    Jugador jugador2(1, "Jugador 2", &mapa);

    jugador1.asignarTanques();
    jugador2.asignarTanques();
    mapa.generarObstaculos(50);

    SistemaTurnos turnos(&jugador1, &jugador2);
    //Pruebas
    jugador1.getCola()->Enlistar(new DobleTurno(&turnos));
    jugador1.getCola()->Enlistar(new PoderAtaque(&turnos));
    jugador2.getCola()->Enlistar(new PrecisionMovimiento(&turnos));
    jugador2.getCola()->Enlistar(new PrecisionAtaque(&turnos));
    //
    Renderer renderer(&mapa, &jugador1, &jugador2);
    if (!renderer.init()) return -1;

    Tanque* tanqueSeleccionado = nullptr;
    bool esperandoDestino = false;
    bool esperandoDisparoDestino = false;

    Bala* balas[MAX_BALAS] = {};
    int numBalas = 0;

    int turnosParaPowerup = rand() % 6 + 4;
    int contadorTurnos = 0;

    while (renderer.isOpen()) {
        // Verificar victoria
        Jugador* ganador = turnos.verificarVictoria();
        if (ganador != nullptr) {
            std::cout << "Gano: " << ganador->getNombre() << std::endl;
            break;
        }

        // Actualizar HUD
        renderer.updateTurnDisplay(turnos.getJugadorActivo());

        // Avanzar movimiento del tanque
        if (tanqueSeleccionado != nullptr && !esperandoDestino && !esperandoDisparoDestino) {
            if (tanqueSeleccionado->enMovimiento()) {
                tanqueSeleccionado->paso();
            }
            else {
                turnos.completarTurno();
                turnos.siguienteTurno();
                contadorTurnos++;
                if (contadorTurnos >= turnosParaPowerup) {
                    turnos.generarPowerupAleatorio();
                    contadorTurnos = 0;
                    turnosParaPowerup = rand() % 5 + 2;
                }
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

                if (nodoClick < 0 || nodoClick >= 400) continue;

                Jugador* activo = turnos.getJugadorActivo();

                // Click izquierdo: seleccionar y mover
                if (mb->button == sf::Mouse::Button::Left) {
                    bool tanqueEncontrado = false;
                        for (int i = 0; i < 4; i++) {
                            Tanque* t = activo->getTanque(i);
                            if (t != nullptr && t->getNodoActual() == nodoClick) {
                                tanqueSeleccionado = t;
                                esperandoDestino = true;
                                esperandoDisparoDestino = false;
                                tanqueEncontrado = true;
                                std::cout << "Tanque seleccionado en nodo " << nodoClick << std::endl;
                                break;
                            }
                        }
                    if (!tanqueEncontrado && esperandoDestino) {
                        if (mapa.disponible(nodoClick)) {
                            tanqueSeleccionado->mover_tanque(nodoClick);
                            esperandoDestino = false;
                            std::cout << "Moviendo a nodo " << nodoClick << std::endl;
                        }
                        else {
                            std::cout << "Destino invalido o bloqueado" << std::endl;
                        }
                    }
                }

                // Click derecho: disparar
                if (mb->button == sf::Mouse::Button::Right) {
                    std::cout << "Click derecho en nodo " << nodoClick << std::endl;
                    bool tanqueEncontrado = false;
                        for (int i = 0; i < 4; i++) {
                            Tanque* t = activo->getTanque(i);
                            if (t != nullptr && t->getNodoActual() == nodoClick) {
                                tanqueSeleccionado = t;
                                esperandoDisparoDestino = true;
                                esperandoDestino = false;
                                tanqueEncontrado = true;
                                std::cout << "Tanque seleccionado para disparar en nodo " << nodoClick << std::endl;
                                break;
                            }
                        }

                    if (!tanqueEncontrado && esperandoDisparoDestino) {
                        if (numBalas < MAX_BALAS) {
                            int nodoTanque = tanqueSeleccionado->getNodoActual();
                            int ancho = mapa.getAncho();
                            int fila = nodoTanque / ancho;
                            int col = nodoTanque % ancho;
                            int filaDestino = nodoClick / ancho;
                            int colDestino = nodoClick % ancho;

                            // Calcular nodo adyacente en dirección al destino
                            int df = 0, dc = 0;
                            if (filaDestino > fila) df = 1;
                            else if (filaDestino < fila) df = -1;
                            else if (colDestino > col) dc = 1;
                            else if (colDestino < col) dc = -1;

                            int nodoOrigen = (fila + df) * ancho + (col + dc);

                            // Verificar que el nodo origen es válido y está libre
                            if (nodoOrigen >= 0 && nodoOrigen < 400 && mapa.disponible(nodoOrigen)) {
                                bool poder = activo->getPoderAtaque();
                                bool precision = activo->getPrecisionAtaque();
                                Bala* bala = new Bala(nodoOrigen, nodoClick, activo->getId(), poder, &mapa, precision);
                                activo->setPoderAtaque(false);
                                activo->setPrecisionAtaque(false);
                                balas[numBalas++] = bala;
                                std::cout << "Disparo desde nodo " << nodoOrigen << " a " << nodoClick << std::endl;
                            }
                            else {
                                std::cout << "No hay espacio para disparar en esa direccion" << std::endl;
                            }
                        }
                        esperandoDisparoDestino = false;
                        tanqueSeleccionado = nullptr;
                        turnos.completarTurno();
                        turnos.siguienteTurno();
                        contadorTurnos++;
                        if (contadorTurnos >= turnosParaPowerup) {
                            turnos.generarPowerupAleatorio();
                            contadorTurnos = 0;
                            turnosParaPowerup = rand() % 5 + 2;
                        }
                    }
                }
            }
            if (event->is<sf::Event::KeyPressed>()) {
                auto* kp = event->getIf<sf::Event::KeyPressed>();
                if (kp->code == sf::Keyboard::Key::LShift || kp->code == sf::Keyboard::Key::RShift) {
                    Jugador* activo = turnos.getJugadorActivo();
                    Powerup* p = activo->getCola()->Desenlistar();
                    if (p != nullptr) {
                        activo->setPowerupPendiente(p);
                        tanqueSeleccionado = nullptr;
                        esperandoDestino = false;
                        esperandoDisparoDestino = false;
                        turnos.completarTurno();
                        turnos.siguienteTurno();
                        contadorTurnos++;
                        if (contadorTurnos >= turnosParaPowerup) {
                            turnos.generarPowerupAleatorio();
                            contadorTurnos = 0;
                            turnosParaPowerup = rand() % 5 + 2;
                        }
                    }
                }
            }
        }

        // Renderizar
        renderer.render();

        // Avanzar balas despues de renderizar
        for (int i = 0; i < numBalas; i++) {
            if (balas[i] != nullptr && balas[i]->estaActivo()) {
                balas[i]->mover_bala();
            }
            else if (balas[i] != nullptr && !balas[i]->estaActivo()) {
                delete balas[i];
                balas[i] = nullptr;
            }
        }
    }

    // Limpiar balas
    for (int i = 0; i < MAX_BALAS; i++) {
        if (balas[i] != nullptr) delete balas[i];
    }

    return 0;
}