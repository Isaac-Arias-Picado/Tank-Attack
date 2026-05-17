#include "Bala.h"
#include "Tanque.h"
#include <cstring>

Bala::Bala(int nodoOrigen, int nodoDestino, int jugador, bool poderAtaque, Graph* grafo) {
    this->nodoActual = nodoOrigen;
    this->nodoDestino = nodoDestino;
    this->jugador = jugador;
    this->poderAtaque = poderAtaque;
    this->grafo = grafo;
    rebotes = 3;
    int ancho = grafo->getAncho();
    int filaOrigen = nodoOrigen / ancho;
    int colOrigen = nodoOrigen % ancho;
    int filaDestino = nodoDestino / ancho;
    int colDestino = nodoDestino % ancho;
    if (filaDestino > filaOrigen) direccionFila = 1;
    else if (filaDestino < filaOrigen) direccionFila = -1;
    else direccionFila = 0;
    if (colDestino > colOrigen) direccionColumna = 1;
    else if (colDestino < colOrigen) direccionColumna = -1;
    else direccionColumna = 0;
    pathActual = movimientoBala(grafo, nodoOrigen, nodoDestino);
}

void Bala::mover_bala() {
    if (pathActual.indiceActual >= pathActual.longitud) {
        if (grafo->getNodo(nodoActual) != nullptr)
            grafo->getNodo(nodoActual)->setObjeto(nullptr);
        activo = false;
        return;
    }

    int siguiente = pathActual.nodos[pathActual.indiceActual];
    Node* nodoSiguiente = grafo->getNodo(siguiente);
    if (nodoSiguiente == nullptr) {
        if (grafo->getNodo(nodoActual) != nullptr)
            grafo->getNodo(nodoActual)->setObjeto(nullptr);
        activo = false;
        return;
    }

    Object* obj = nodoSiguiente->getObjeto();
    if (obj != nullptr) {
        if (strcmp(obj->getTipo(), "Tanque") == 0) {
            impacto();
            return; 
        }
        else if (strcmp(obj->getTipo(), "Obstaculo") == 0) {
            rebotar();
            return; 
        }
    }
    else {
        Node* nodoActualPtr = grafo->getNodo(nodoActual);
        if (nodoActualPtr != nullptr) nodoActualPtr->setObjeto(nullptr);
        nodoActual = siguiente;
        Node* nodoNuevoPtr = grafo->getNodo(nodoActual);
        if (nodoNuevoPtr != nullptr) nodoNuevoPtr->setObjeto(this);
        pathActual.indiceActual++;
    }
}

void Bala::rebotar() {
    if (rebotes <= 0) {
        activo = false;
        if (grafo->getNodo(nodoActual) != nullptr)
            grafo->getNodo(nodoActual)->setObjeto(nullptr);
        return;
    }
    rebotes--;
    int siguiente = pathActual.nodos[pathActual.indiceActual];
    int ancho = grafo->getAncho();
    int filaActual = nodoActual / ancho;
    int colActual = nodoActual % ancho;
    int filaSiguiente = siguiente / ancho;
    int colSiguiente = siguiente % ancho;
    if (filaActual != filaSiguiente) direccionFila *= -1;
    if (colActual != colSiguiente) direccionColumna *= -1;
    int filaTemp = filaActual + direccionFila;
    int colTemp = colActual + direccionColumna;
    int largo = grafo->getLargo();
    while (filaTemp >= 0 && filaTemp < largo && colTemp >= 0 && colTemp < ancho) {
        int nodoTemp = filaTemp * ancho + colTemp;
        if (!grafo->disponible(nodoTemp)) break;
        filaTemp += direccionFila;
        colTemp += direccionColumna;
    }
    int nuevoDestino = (filaTemp - direccionFila) * ancho + (colTemp - direccionColumna);
    pathActual = movimientoBala(grafo, nodoActual, nuevoDestino);
}

void Bala::impacto() {
    if (pathActual.indiceActual < 0 || pathActual.indiceActual >= pathActual.longitud) {
        if (grafo->getNodo(nodoActual) != nullptr)
            grafo->getNodo(nodoActual)->setObjeto(nullptr);
        activo = false;
        return;
    }

    Node* nodoImp = grafo->getNodo(pathActual.nodos[pathActual.indiceActual]);
    if (nodoImp == nullptr) {
        if (grafo->getNodo(nodoActual) != nullptr)
            grafo->getNodo(nodoActual)->setObjeto(nullptr);
        activo = false;
        return;
    }

    Object* obj = nodoImp->getObjeto();
    if (obj != nullptr && strcmp(obj->getTipo(), "Tanque") == 0) {
        Tanque* tanque = (Tanque*)obj;
        if (poderAtaque) {
            tanque->recibirdanhototal();
        }
        else {
            tanque->recibirdanho();
        }
        activo = false;
        if (grafo->getNodo(nodoActual) != nullptr)
            grafo->getNodo(nodoActual)->setObjeto(nullptr);
        return;
    }
    if (grafo->getNodo(nodoActual) != nullptr)
        grafo->getNodo(nodoActual)->setObjeto(nullptr);
    activo = false;
    return;
}

const char* Bala::getTipo() const {
    return "Bala";
}