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
};