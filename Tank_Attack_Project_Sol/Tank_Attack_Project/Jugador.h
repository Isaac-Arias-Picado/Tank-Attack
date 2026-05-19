#pragma once

class Tanque;
class Graph;

class Jugador {
private:
    Graph* grafo;
    int id;
    char nombre[50];
    Tanque** tanques;
    int tanquesVivos;

public:
    int getId() const { return id; }
    const char* getNombre() const { return nombre; }
    Jugador(int id, const char* nombre, Graph* grafo);
    void asignarTanques();
    Tanque* getTanque(int index) const;
    int getTanquesVivos();
    void tanqueMuerto();
};