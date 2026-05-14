// Obstaculo.h
#pragma once
#include "Object.h"

class Obstaculo : public Object {
public:
    const char* getTipo() const override { return "Obstaculo"; }
};
