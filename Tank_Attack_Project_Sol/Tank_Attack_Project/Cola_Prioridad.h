struct NodoHeap {
	int idnodo;
	float costo;
};

class ColaPrioridad {
private:
	NodoHeap datos[400];
	int tamanio;
public:
	ColaPrioridad();
	void Insertar(int idnodo, float costo);
	NodoHeap Sacarmin();
	bool vacio();
};
