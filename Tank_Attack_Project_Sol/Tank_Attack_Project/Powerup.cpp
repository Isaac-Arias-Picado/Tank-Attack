#include "Powerup.h"

Powerup::Powerup(const char* nombre) {
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