#include "Cola.h"
#include "Powerup.h"

Cola::Cola() {
	this->cabeza = nullptr;
	this->frente = nullptr;
	this->cantnodos = 0;
}
//Metodo para agregar nuevos power ups a la cola
void Cola::Enlistar(Powerup* nuevo) {
	NodoCola* nuevonodo = new NodoCola ();
	nuevonodo->dato = nuevo;
	nuevonodo->siguiente = nullptr;
	if (cabeza == nullptr) {
		cabeza = nuevonodo;
		frente = nuevonodo;
	}
	else {
		frente->siguiente = nuevonodo;
		frente = nuevonodo;
	}
	cantnodos++;
}
//Metodo para eliminar el primer elemento de la cola (FIFO) y retorna el power up para usarlo por el jugador
Powerup* Cola::Desenlistar() {
	if (cabeza == nullptr) return nullptr;
	NodoCola* temp = cabeza;
	Powerup* powerup=nullptr;
	powerup=temp->dato;
	cabeza = cabeza->siguiente;
	delete temp;
	temp = nullptr;
	cantnodos--;
	return powerup;
}