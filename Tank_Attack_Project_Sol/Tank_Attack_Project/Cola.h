#pragma once
class Powerup;

struct NodoCola {
	Powerup* dato;
	NodoCola* siguiente;
}; 

class Cola {
private:
	NodoCola* cabeza;
	NodoCola* frente;
	int cantnodos;

public:
	Cola();
	void Enlistar(Powerup* nuevo);
	Powerup* Desenlistar();
	Powerup* getPowerup(int indice);
	int getCantNodos();
};