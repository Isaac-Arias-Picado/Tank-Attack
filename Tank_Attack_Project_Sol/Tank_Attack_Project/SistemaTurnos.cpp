#include "SistemaTurnos.h"
#include "Jugador.h"

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
			if (jugadorActivo == 0) {
				jugadorActivo = 1;
			}
			else {
				jugadorActivo = 0;
			}
		}
		turnoCompletado = false;
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



