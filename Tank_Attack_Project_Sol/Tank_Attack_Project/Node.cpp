#include "Node.h"

Node::Node(int id) {
	this->id = id;
	this->objeto = nullptr;
}

Object* Node::getObjeto() {
	return objeto;
}

void Node::setObjeto(Object* obj) {
	objeto = obj;
}