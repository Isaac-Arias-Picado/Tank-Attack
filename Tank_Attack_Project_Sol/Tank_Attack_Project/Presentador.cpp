#include "Presentador.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Presentador::Presentador()
    : mapa(20, 20),
    jugador1(0, "Jugador 1", &mapa),
    jugador2(1, "Jugador 2", &mapa),
    turnos(&jugador1, &jugador2),
    renderer(&mapa, &jugador1, &jugador2),
    tanqueSeleccionado(nullptr),
    esperandoDestino(false),
    esperandoDisparoDestino(false),
    numBalas(0),
    contadorTurnos(0),
    velocidadTanque(0.1f),
    velocidadBala(0.1f)
{
    for (int i = 0; i < MAX_BALAS; i++) balas[i] = nullptr;
    turnosParaPowerup = rand() % 5 + 2;
}

bool Presentador::init() {
    jugador1.asignarTanques();
    jugador2.asignarTanques();
    mapa.generarObstaculos(50);

    // Pruebas
    jugador1.getCola()->Enlistar(new DobleTurno(&turnos));
    jugador1.getCola()->Enlistar(new PoderAtaque(&turnos));
    jugador2.getCola()->Enlistar(new PrecisionMovimiento(&turnos));
    jugador2.getCola()->Enlistar(new PrecisionAtaque(&turnos));

    return renderer.init();
}

void Presentador::run() {
    while (renderer.isOpen()) {
        Jugador* ganador = turnos.verificarVictoria();
        if (ganador != nullptr || turnos.tiempoAgotado()) {
            renderer.mostrarVictoria(ganador);
            break;
        }

        renderer.updateHUD(turnos.getJugadorActivo(), turnos.getTiempoRestante());

        avanzarTanque();
        procesarEventos();
        renderer.render();
        avanzarBalas();
    }

    for (int i = 0; i < MAX_BALAS; i++) {
        if (balas[i] != nullptr) delete balas[i];
    }
}

void Presentador::avanzarTurno() {
    turnos.completarTurno();
    turnos.siguienteTurno();
    contadorTurnos++;
    if (contadorTurnos >= turnosParaPowerup) {
        turnos.generarPowerupAleatorio();
        contadorTurnos = 0;
        turnosParaPowerup = rand() % 5 + 2;
    }
}

void Presentador::avanzarTanque() {
    if (tanqueSeleccionado == nullptr || esperandoDestino || esperandoDisparoDestino) return;

    if (tanqueSeleccionado->enMovimiento()) {
        if (relojMovimiento.getElapsedTime().asSeconds() > velocidadTanque) {
            tanqueSeleccionado->paso();
            relojMovimiento.restart();
        }
    }
    else {
        avanzarTurno();
        tanqueSeleccionado = nullptr;
        esperandoDestino = false;
    }
}

void Presentador::avanzarBalas() {
    for (int i = 0; i < numBalas; i++) {
        if (balas[i] != nullptr && balas[i]->estaActivo()) {
            if (relojBala.getElapsedTime().asSeconds() > velocidadBala) {
                balas[i]->mover_bala();
                relojBala.restart();
                if (balas[i]->getRebotoReciente()) {
                    renderer.setRutaBala(balas[i]->getPath());
                    balas[i]->setRebotoReciente(false);
                }
            }
        }
        else if (balas[i] != nullptr && !balas[i]->estaActivo()) {
            delete balas[i];
            balas[i] = nullptr;
        }
    }
}

void Presentador::procesarEventos() {
    while (const std::optional event = renderer.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            renderer.close();
        }

        if (event->is<sf::Event::MouseButtonPressed>()) {
            auto* mb = event->getIf<sf::Event::MouseButtonPressed>();
            sf::Vector2i pixelPos(mb->position.x, mb->position.y);
            sf::Vector2f worldPos = renderer.getWindow().mapPixelToCoords(pixelPos);
            int col = (int)worldPos.x / 64;
            int row = (int)worldPos.y / 64;
            int nodoClick = row * 20 + col;

            if (nodoClick < 0 || nodoClick >= 400) continue;

            if (mb->button == sf::Mouse::Button::Left)  onClickIzquierdo(nodoClick);
            if (mb->button == sf::Mouse::Button::Right) onClickDerecho(nodoClick);
        }

        if (event->is<sf::Event::KeyPressed>()) {
            auto* kp = event->getIf<sf::Event::KeyPressed>();
            if (kp->code == sf::Keyboard::Key::LShift || kp->code == sf::Keyboard::Key::RShift)
                onShiftPresionado();
        }
    }
}

void Presentador::onClickIzquierdo(int nodoClick) {
    Jugador* activo = turnos.getJugadorActivo();

    for (int i = 0; i < 4; i++) {
        Tanque* t = activo->getTanque(i);
        if (t != nullptr && t->getNodoActual() == nodoClick) {
            tanqueSeleccionado = t;
            esperandoDestino = true;
            esperandoDisparoDestino = false;
            std::cout << "Tanque seleccionado en nodo " << nodoClick << std::endl;
            return;
        }
    }

    if (esperandoDestino) {
        if (mapa.disponible(nodoClick)) {
            tanqueSeleccionado->mover_tanque(nodoClick);
            renderer.setRutaTanque(tanqueSeleccionado->getPath());
            esperandoDestino = false;
            std::cout << "Moviendo a nodo " << nodoClick << std::endl;
        }
        else {
            std::cout << "Destino invalido o bloqueado" << std::endl;
        }
    }
}

void Presentador::onClickDerecho(int nodoClick) {
    Jugador* activo = turnos.getJugadorActivo();
    std::cout << "Click derecho en nodo " << nodoClick << std::endl;

    for (int i = 0; i < 4; i++) {
        Tanque* t = activo->getTanque(i);
        if (t != nullptr && t->getNodoActual() == nodoClick) {
            tanqueSeleccionado = t;
            esperandoDisparoDestino = true;
            esperandoDestino = false;
            std::cout << "Tanque seleccionado para disparar en nodo " << nodoClick << std::endl;
            return;
        }
    }

    if (esperandoDisparoDestino) {
        if (numBalas < MAX_BALAS) {
            int nodoTanque = tanqueSeleccionado->getNodoActual();
            int ancho = mapa.getAncho();
            int fila = nodoTanque / ancho;
            int col = nodoTanque % ancho;
            int filaDestino = nodoClick / ancho;
            int colDestino = nodoClick % ancho;

            int df = 0, dc = 0;
            if (filaDestino > fila) df = 1;
            else if (filaDestino < fila) df = -1;
            else if (colDestino > col)  dc = 1;
            else if (colDestino < col)  dc = -1;

            int nodoOrigen = (fila + df) * ancho + (col + dc);

            if (nodoOrigen >= 0 && nodoOrigen < 400 && mapa.disponible(nodoOrigen)) {
                bool poder = activo->getPoderAtaque();
                bool precision = activo->getPrecisionAtaque();
                Bala* bala = new Bala(nodoOrigen, nodoClick, activo->getId(), poder, &mapa, precision);
                renderer.setRutaBala(bala->getPath());
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
        avanzarTurno();
    }
}

void Presentador::onShiftPresionado() {
    Jugador* activo = turnos.getJugadorActivo();
    Powerup* p = activo->getCola()->Desenlistar();
    if (p != nullptr) {
        activo->setPowerupPendiente(p);
        tanqueSeleccionado = nullptr;
        esperandoDestino = false;
        esperandoDisparoDestino = false;
        avanzarTurno();
    }
}