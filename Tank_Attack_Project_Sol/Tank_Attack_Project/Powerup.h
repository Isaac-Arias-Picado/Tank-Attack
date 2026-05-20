
#include <cstring>

class Jugador;
class SistemaTurnos;

class Powerup {
protected:
    char nombre[50];
    SistemaTurnos* turno;
public:
    Powerup(const char* nombre, SistemaTurnos* turno);
    virtual void aplicar(Jugador* jugador) = 0;
    virtual ~Powerup() {}
    const char* getNombre();
};

class DobleTurno : public Powerup {
public:
    DobleTurno(SistemaTurnos* turno) : Powerup("DobleTurno", turno) {}
    void aplicar(Jugador* jugador) override;
};

class PrecisionMovimiento : public Powerup {
public:
    PrecisionMovimiento(SistemaTurnos* turno) : Powerup("PrecisionMovimiento", turno) {}
    void aplicar(Jugador* jugador) override;
};

class PrecisionAtaque : public Powerup {
public:
    PrecisionAtaque(SistemaTurnos* turno) : Powerup("PrecisionAtaque", turno) {}
    void aplicar(Jugador* jugador) override;
};

class PoderAtaque : public Powerup {
public:
    PoderAtaque(SistemaTurnos* turno) : Powerup("PoderAtaque", turno) {}
    void aplicar(Jugador* jugador) override;
};