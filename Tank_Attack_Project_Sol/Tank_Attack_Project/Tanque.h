#include "Object.h"
class Graph;
class Tanque : public Object {
private:
    int jugador;
    int color;
    float vida;
    float vidaMaxima;
    int nodoActual;
    Graph* grafo;

public:
    Tanque(int jugador, int color, int nodoInicial,Graph* grafo);
    const char* getTipo() const override;  
	void mover_tanque(int nuevoNodo);
    int getNodoActual();
};