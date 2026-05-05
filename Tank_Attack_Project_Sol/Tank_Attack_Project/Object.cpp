#include "Object.h"

Object::Object() : id(0), activo(true) {}
Object::~Object() {}
int Object::getId() const {return id;}
bool Object::isActivo() const {return activo;}
void Object::setActivo(bool activo) {this->activo = activo;}