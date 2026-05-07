#include "Cola_Prioridad.h"

ColaPrioridad::ColaPrioridad() {
	tamanio = 0;
}

void ColaPrioridad::Insertar(int idnodo, float costo) {
	if (tamanio >= 400) return;
	datos[tamanio].idnodo = idnodo;
	datos[tamanio].costo = costo;
	tamanio++;
	int i = tamanio - 1;
	while (i>0 && datos[i].costo < datos[(i - 1) / 2].costo) {
		NodoHeap temp = datos[(i - 1) / 2];
		datos[(i - 1) / 2] = datos[i];
		datos[i] = temp;
		i = (i - 1) / 2;
	}
}

NodoHeap ColaPrioridad::Sacarmin() {
	if (vacio()) return {-1, -1.0f};
	NodoHeap min = datos[0];
	datos[0] = datos[tamanio - 1];
	tamanio--;
	int i = 0;
	int hijoizquierdo = 2 * i + 1;
	int hijoderecho = 2 * i + 2;
	while (i < tamanio && hijoizquierdo<tamanio) {
		hijoizquierdo = 2 * i + 1;
		hijoderecho = 2 * i + 2;
		// Si solo existe el hijo izquerdo, solo se compara el padre con este
		if (hijoderecho >= tamanio) {
			if (datos[i].costo > datos[hijoizquierdo].costo) {
				NodoHeap temp = datos[i];
				datos[i] = datos[hijoizquierdo];
				datos[hijoizquierdo] = temp;
				i = hijoizquierdo;
			}
			//Sale del bucle, ya que como no hay hijo derecho no hay mas que comparar
			break;
		}
		//Se analiza el caso donde el hijo izquierdo es menor
		if (datos[hijoizquierdo].costo <= datos[hijoderecho].costo && datos[i].costo>datos[hijoizquierdo].costo) {
			NodoHeap temp = datos[i];
			datos[i] = datos[hijoizquierdo];
			datos[hijoizquierdo] = temp;
			i = hijoizquierdo;
		}
		else {
			//Si hijo derecho es el menor, se analiza si este es menor que su padre
			if (datos[i].costo > datos[hijoderecho].costo) {
				NodoHeap temp = datos[i];
				datos[i] = datos[hijoderecho];
				datos[hijoderecho] = temp;
				i = hijoderecho;
			}
			else {
				break;
			}
		}
	}
	return min;
}

bool ColaPrioridad::vacio() {
	return tamanio == 0;
}