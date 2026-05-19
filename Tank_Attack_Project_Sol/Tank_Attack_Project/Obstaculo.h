// Obstaculo.h
#pragma once
#include "Object.h"
class Obstaculo : public Object {
public:
    Obstaculo() : Object() {}
    const char* getTipo() const override { return "Obstaculo"; }
};