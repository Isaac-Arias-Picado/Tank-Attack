#pragma once

class Tanque;
class Graph;
class Powerup;
class Cola;

class Jugador {
private:
    Graph* grafo;
    int id;
    char nombre[50];
    Tanque** tanques;
    int tanquesVivos;
    bool precisionMovimiento;
    bool precisionAtaque;
    bool poderAtaque;
    Powerup* powerupPendiente;
    Cola* colaPowerups;

public:
    int getId() const { return id; }
    const char* getNombre() const { return nombre; }
    Jugador(int id, const char* nombre, Graph* grafo);
    void asignarTanques();
    Tanque* getTanque(int index) const;
    int getTanquesVivos();
    void tanqueMuerto();
    Cola* getCola();
    void setPrecisionMovimiento(bool valor);
    void setPrecisionAtaque(bool valor);
    void setPoderAtaque(bool valor);
    bool getPrecisionMovimiento();
    bool getPrecisionAtaque();
    bool getPoderAtaque();
    void setPowerupPendiente(Powerup* p);
    Powerup* getPowerupPendiente();
};