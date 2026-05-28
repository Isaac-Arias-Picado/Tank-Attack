#include "SistemaTurnos.h"
#include "Jugador.h"
#include "Powerup.h"
#include "Cola.h"

SistemaTurnos::SistemaTurnos(Jugador* j1,Jugador* j2) {
	jugadores[0] = j1;
	jugadores[1] = j2;
	jugadorActivo = 0;
	turnoCompletado = false;
	turnosExtra = 0;
	tiempoLimite = 300.0f;
}

void SistemaTurnos::siguienteTurno() {
	if (turnoCompletado) {
		if (turnosExtra > 0) {
			turnosExtra--;
		}
		else {
			if (jugadorActivo == 0) jugadorActivo = 1;
			else jugadorActivo = 0;
		}
		turnoCompletado = false;

		Jugador* actual = getJugadorActivo();
		if (actual->getPowerupPendiente() != nullptr) {
			actual->getPowerupPendiente()->aplicar(actual);
			actual->setPowerupPendiente(nullptr);
		}
	}
}

void SistemaTurnos::completarTurno() {
	turnoCompletado=true;
}

bool SistemaTurnos::tiempoAgotado() {
	return reloj.getElapsedTime().asSeconds() >= tiempoLimite;
}

Jugador* SistemaTurnos::verificarVictoria() {
	if (jugadores[0]->getTanquesVivos() == 0) {
		return jugadores[1];
	}
	if (jugadores[1]->getTanquesVivos() == 0) {
		return jugadores[0];
	}
	if (tiempoAgotado() && jugadores[0]->getTanquesVivos()<jugadores[1]->getTanquesVivos()) {
		return jugadores[1];
	}
	else if (tiempoAgotado() && jugadores[0]->getTanquesVivos() > jugadores[1]->getTanquesVivos())
		return jugadores[0];
	else {
		return nullptr;
	}
}

Jugador* SistemaTurnos::getJugadorActivo() {
	if (jugadorActivo == 0) {
		return jugadores[0];
	}
	else {
		return jugadores[1];
	}
}

void SistemaTurnos::generarPowerupAleatorio() {
	int jugador = rand() % 2;
	int tipo = rand() % 4;
	Powerup* p = nullptr;
	if (tipo == 0) p = new DobleTurno(this);
	else if (tipo == 1) p = new PrecisionMovimiento(this);
	else if (tipo == 2) p = new PrecisionAtaque(this);
	else p = new PoderAtaque(this);
	jugadores[jugador]->getCola()->Enlistar(p);
}

float SistemaTurnos::getTiempoRestante() {
	float restante = tiempoLimite - reloj.getElapsedTime().asSeconds();
	if (restante < 0) return 0;
	return restante;
}

void SistemaTurnos::agregarTurnoExtra() {
	turnosExtra++;
}



