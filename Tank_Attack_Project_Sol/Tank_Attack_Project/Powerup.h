class Jugador;
class Powerup {
private:
	char nombre[50];
public:
	Powerup(const char* nombre);
	const char* getNombre();
	virtual void aplicar(Jugador* jugador) = 0;
	virtual ~Powerup() {};
};