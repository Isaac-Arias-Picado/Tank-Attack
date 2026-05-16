#include "Tanque.h"
class Graph;

class Jugador {
private:
    Graph* grafo;
    int id;                   // 0 o 1
    char nombre[50];          // nombre del jugador
    Tanque** tanques;         // arreglo de 4 tanques
    int tanquesVivos;   

public:
    Jugador(int id, const char* nombre, Graph* grafo);
    void asignarTanques();
    Tanque* getTanque(int index) const;
    int getTanquesVivos();
};

