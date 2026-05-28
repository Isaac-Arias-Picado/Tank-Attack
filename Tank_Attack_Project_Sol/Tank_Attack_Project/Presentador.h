#pragma once
#include <SFML/Graphics.hpp>
#include "Graph.h"
#include "Jugador.h"
#include "Renderer.h"
#include "SistemaTurnos.h"
#include "Tanque.h"
#include "Bala.h"
#include "Cola.h"
#include "Powerup.h"

const int MAX_BALAS = 20;

class Presentador {
public:
    Presentador();
    bool init();
    void run();

private:
    Graph mapa;
    Jugador jugador1;
    Jugador jugador2;
    SistemaTurnos turnos;
    Renderer renderer;

    Tanque* tanqueSeleccionado;
    bool esperandoDestino;
    bool esperandoDisparoDestino;

    Bala* balas[MAX_BALAS];
    int numBalas;

    int turnosParaPowerup;
    int contadorTurnos;

    sf::Clock relojMovimiento;
    float velocidadTanque;
    sf::Clock relojBala;
    float velocidadBala;

    void avanzarTanque();
    void avanzarBalas();
    void procesarEventos();
    void onClickIzquierdo(int nodoClick);
    void onClickDerecho(int nodoClick);
    void onShiftPresionado();
    void avanzarTurno();
};
