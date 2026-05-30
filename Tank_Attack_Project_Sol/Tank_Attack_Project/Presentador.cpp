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
    balaActual(nullptr),
    turnoPendiente(false),
    contadorTurnos(0),
    velocidadTanque(0.1f),
    velocidadBala(0.1f),
    estadoMov(0),
    nodoTeletransporte(-1)
{
    turnosParaPowerup = rand() % 5 + 2;
}

bool Presentador::init() {
    jugador1.asignarTanques();
    jugador2.asignarTanques();
    mapa.generarObstaculos(50);

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

    if (balaActual != nullptr) delete balaActual;
}

void Presentador::avanzarTurno() {
    renderer.limpiarRutas();
    turnos.completarTurno();
    turnos.siguienteTurno();
    renderer.limpiarUltimoPowerup(turnos.getJugadorActivo()->getId());
    contadorTurnos++;
    if (contadorTurnos >= turnosParaPowerup) {
        turnos.generarPowerupAleatorio();
        contadorTurnos = 0;
        turnosParaPowerup = rand() % 5 + 2;
    }
}

void Presentador::avanzarTanque() {
    if (tanqueSeleccionado == nullptr || esperandoDestino || esperandoDisparoDestino) return;

    if (estadoMov == 1) {
        if (relojEspera.getElapsedTime().asSeconds() >= 0.5f) {
            estadoMov = 2;
            renderer.limpiarRutas();
            renderer.setRutaTanque(rutaFinal);
            renderer.setNodoTeleporte(nodoTeletransporte);
        }
        return;
    }

    if (estadoMov == 2) {
        renderer.limpiarNodoTeleporte();
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
            estadoMov = 0;
            nodoTeletransporte = -1;
            renderer.limpiarNodoTeleporte();
        }
    }
}

void Presentador::avanzarBalas() {
    // Si no hay bala, no hacer nada
    if (balaActual == nullptr) return;

    // Si la bala está activa, moverla
    if (balaActual->estaActivo()) {
        if (relojBala.getElapsedTime().asSeconds() > velocidadBala) {
            balaActual->mover_bala();
            relojBala.restart();

            // Actualizar ruta mientras se mueve
            renderer.setRutaBala(balaActual->getPath());

            if (balaActual->getRebotoReciente()) {
                balaActual->setRebotoReciente(false);
            }
        }
    }
    else {
        // Bala terminó su movimiento
        delete balaActual;
        balaActual = nullptr;

        // Avanzar turno si estaba pendiente
        if (turnoPendiente) {
            turnoPendiente = false;
            avanzarTurno();
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
            nodoTeletransporte = -1;
            renderer.limpiarNodoTeleporte();
            return;
        }
    }

    if (esperandoDestino) {
        if (mapa.disponible(nodoClick)) {
            tanqueSeleccionado->mover_tanque(nodoClick);
            Path p = tanqueSeleccionado->getPath();
            esperandoDestino = false;

            if (p.fueBloqueado && p.nodoTeletransporte != -1) {
                rutaParcial = Path();
                for (int i = 0; i < p.longitud; i++) {
                    rutaParcial.nodos[rutaParcial.longitud++] = p.nodos[i];
                    if (p.nodos[i] == p.nodoTeletransporte) break;
                }
                rutaFinal = Path();
                bool encontrado = false;
                for (int i = 0; i < p.longitud; i++) {
                    if (p.nodos[i] == p.nodoTeletransporte) encontrado = true;
                    if (encontrado) rutaFinal.nodos[rutaFinal.longitud++] = p.nodos[i];
                }
                rutaFinal.indiceActual = 1;
                nodoTeletransporte = p.nodoTeletransporte;
                renderer.setRutaTanque(rutaParcial);
                renderer.setNodoTeleporte(nodoTeletransporte); 
                estadoMov = 1;
                relojEspera.restart();
            }
            else {

                nodoTeletransporte = -1;
                renderer.limpiarNodoTeleporte(); 
                renderer.setRutaTanque(p);
                estadoMov = 2;
            }
        }
        else {
        }
    }
}

void Presentador::onClickDerecho(int nodoClick) {
    Jugador* activo = turnos.getJugadorActivo();

    for (int i = 0; i < 4; i++) {
        Tanque* t = activo->getTanque(i);
        if (t != nullptr && t->getNodoActual() == nodoClick) {
            tanqueSeleccionado = t;
            esperandoDisparoDestino = true;
            esperandoDestino = false;
            return;
        }
    }

    if (esperandoDisparoDestino) {
        // Solo disparar si no hay bala activa
        if (balaActual == nullptr) {
            int nodoTanque = tanqueSeleccionado->getNodoActual();
            int ancho = mapa.getAncho();
            int fila = nodoTanque / ancho;
            int col = nodoTanque % ancho;
            int filaDestino = nodoClick / ancho;
            int colDestino = nodoClick % ancho;

            int df = 0, dc = 0;
            if (filaDestino > fila) df = 1;
            else if (filaDestino < fila) df = -1;
            if (colDestino > col) dc = 1;
            else if (colDestino < col) dc = -1;

            int nodoOrigen = (fila + df) * ancho + (col + dc);

            Node* nodoOrig = mapa.getNodo(nodoOrigen);
            Object* obj = nodoOrig ? nodoOrig->getObjeto() : nullptr;
            bool esObstaculo = obj != nullptr && strcmp(obj->getTipo(), "Obstaculo") == 0;

            if (nodoOrigen >= 0 && nodoOrigen < 400 && !esObstaculo) {
                bool poder = activo->getPoderAtaque();
                bool precision = activo->getPrecisionAtaque();
                balaActual = new Bala(nodoOrigen, nodoClick, activo->getId(), poder, &mapa, precision);
                renderer.setRutaBala(balaActual->getPath());
                activo->setPoderAtaque(false);
                activo->setPrecisionAtaque(false);
                esperandoDisparoDestino = false;
                tanqueSeleccionado = nullptr;
                turnoPendiente = true;
            }
            else {
            }
        }
        else {
        }
    }
}

void Presentador::onShiftPresionado() {
    Jugador* activo = turnos.getJugadorActivo();
    Powerup* p = activo->getCola()->Desenlistar();
    if (p != nullptr) {
        renderer.setUltimoPowerup(activo->getId(), p->getNombre());
        activo->setPowerupPendiente(p);
        tanqueSeleccionado = nullptr;
        esperandoDestino = false;
        esperandoDisparoDestino = false;
        avanzarTurno();
    }
}