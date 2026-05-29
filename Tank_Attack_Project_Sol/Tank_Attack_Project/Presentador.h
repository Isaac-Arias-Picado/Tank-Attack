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

    Bala* balaActual;  
    bool turnoPendiente;

    int turnosParaPowerup;
    int contadorTurnos;

    sf::Clock relojMovimiento;
    float velocidadTanque;
    sf::Clock relojBala;
    float velocidadBala;

    int estadoMov;
    sf::Clock relojEspera;
    Path rutaParcial;
    int nodoTeletransporte;
    Path rutaFinal;

    void avanzarTanque();
    void avanzarBalas();
    void procesarEventos();
    void onClickIzquierdo(int nodoClick);
    void onClickDerecho(int nodoClick);
    void onShiftPresionado();
    void avanzarTurno();
};