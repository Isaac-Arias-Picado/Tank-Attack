class Node;
class Object;

class Graph {
private:
    int n; //cantidad de nodos
	int largo;
	int ancho;
    bool** matrizAdy;
    Node** nodos;

public:
    Graph(int largo, int ancho);
	bool disponible(int nodoId);
    Node* getNodo(int id);
};
