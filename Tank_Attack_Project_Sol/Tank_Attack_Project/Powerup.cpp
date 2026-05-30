#include "Powerup.h"
#include "SistemaTurnos.h"
#include "Jugador.h"
#include <iostream>

Powerup::Powerup(const char* nombre, SistemaTurnos* turno) {
    this->turno = turno;
    int i = 0;
    while (nombre[i] != '\0' && i < 49) {
        this->nombre[i] = nombre[i];
        i++;
    }
    this->nombre[i] = '\0';
}

const char* Powerup::getNombre() {
    return nombre;
}

void DobleTurno::aplicar(Jugador* jugador) {
    turno->agregarTurnoExtra();
    std::cout << "DobleTurno aplicado" << std::endl;
}

void PrecisionMovimiento::aplicar(Jugador* jugador) {
    jugador->setPrecisionMovimiento(true);
    std::cout << "PrecisionMovimiento aplicado" << std::endl;
}

void PrecisionAtaque::aplicar(Jugador* jugador) {
    jugador->setPrecisionAtaque(true);
    std::cout << "PrecisionAtaque aplicado" << std::endl;
}

void PoderAtaque::aplicar(Jugador* jugador) {
    jugador->setPoderAtaque(true);
    std::cout << "PoderAtaque aplicado" << std::endl;
}