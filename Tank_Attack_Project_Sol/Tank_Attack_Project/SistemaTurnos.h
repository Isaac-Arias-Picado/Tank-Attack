#include <SFML/Graphics.hpp>
class Jugador;
class SistemaTurnos {
public:
	Jugador* jugadores[2];
	int jugadorActivo;
	bool turnoCompletado;
	int turnosExtra;
	sf::Clock reloj;
	float tiempoLimite;
private:
	SistemaTurnos(Jugador* j1, Jugador* j2);
	void siguienteTurno();
	void completarTurno();
	Jugador* verificarVictoria();
	bool tiempoAgotado();
	Jugador* getJugadorActivo();
};