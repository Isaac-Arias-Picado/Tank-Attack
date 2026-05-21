#include "Bala.h"
#include "Tanque.h"
#include <cstring>
#include <iostream>

Bala::Bala(int nodoOrigen, int nodoDestino, int jugador, bool poderAtaque, Graph* grafo, bool precisionAtaque){
    this->precisionAtaque = precisionAtaque;
    this->nodoActual = nodoOrigen;
    this->nodoDestino = nodoDestino;
    this->jugador = jugador;
    this->poderAtaque = poderAtaque;
    this->grafo = grafo;
    rebotes = 3;
    rebotoReciente = false;
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
    activo = true;
    grafo->getNodo(nodoActual)->setObjeto(this);
    activo = true;
    Node* nodoInicial = grafo->getNodo(nodoActual);
    if (nodoInicial != nullptr && nodoInicial->getObjeto() == nullptr) {
        nodoInicial->setObjeto(this);
    }
    if (precisionAtaque) {
        pathActual = AAsterisco(grafo, nodoOrigen, nodoDestino);
    }
    else {
        pathActual = movimientoBala(grafo, nodoOrigen, nodoDestino);
    }
}

void Bala::mover_bala() {
    std::cout << "mover_bala: indiceActual=" << pathActual.indiceActual << " longitud=" << pathActual.longitud << std::endl;
    if (!activo) return;

    if (pathActual.longitud == 0 || pathActual.indiceActual >= pathActual.longitud) {
        // Limpiar nodo actual
        Node* nodoActualPtr = grafo->getNodo(nodoActual);
        if (nodoActualPtr != nullptr) {
            Object* objActual = nodoActualPtr->getObjeto();
            if (objActual != nullptr && strcmp(objActual->getTipo(), "Bala") == 0)
                nodoActualPtr->setObjeto(nullptr);
        }
        rebotar();
        return;
    }

    int siguiente = pathActual.nodos[pathActual.indiceActual];

    // Validar indice
    if (siguiente < 0 || siguiente >= grafo->getN()) {
        activo = false;
        return;
    }

    Node* nodoSiguiente = grafo->getNodo(siguiente);
    if (nodoSiguiente == nullptr) {
        activo = false;
        return;
    }

    Object* obj = nodoSiguiente->getObjeto();
    if (obj != nullptr) {
        const char* tipo = obj->getTipo();
        if (tipo != nullptr && strcmp(tipo, "Tanque") == 0) {
            impacto(siguiente);
            return;
        }
        else if (tipo != nullptr && strcmp(tipo, "Obstaculo") == 0) {
            std::cout << "Obstaculo detectado en nodo: " << siguiente << std::endl;
            rebotar();
            return;
        }
        else if (tipo != nullptr && strcmp(tipo, "Bala") == 0) {
        }
    }

    //Limpiar nodo actual solo si la bala lo ocupa
    Node* nodoActualPtr = grafo->getNodo(nodoActual);
    if (nodoActualPtr != nullptr) {
        Object* objActual = nodoActualPtr->getObjeto();
        if (objActual != nullptr && strcmp(objActual->getTipo(), "Bala") == 0)
            nodoActualPtr->setObjeto(nullptr);
    }

    nodoActual = siguiente;

    //Ocupar nuevo nodo solo si está libre
    if (nodoSiguiente->getObjeto() == nullptr)
        nodoSiguiente->setObjeto(this);

    pathActual.indiceActual++;
}

void Bala::rebotar() {
    // Limpiar nodo actual
    Node* nodoActualPtr = grafo->getNodo(nodoActual);
    if (nodoActualPtr != nullptr) {
        Object* objActual = nodoActualPtr->getObjeto();
        if (objActual != nullptr && strcmp(objActual->getTipo(), "Bala") == 0)
            nodoActualPtr->setObjeto(nullptr);
    }

    if (rebotes <= 0) {
        activo = false;
        return;
    }

    int ancho = grafo->getAncho();
    int largo = grafo->getLargo();
    int filaActual = nodoActual / ancho;
    int colActual = nodoActual % ancho;

    if (filaActual == 0 || filaActual == largo - 1) {
        direccionFila *= -1;
    }
    if (colActual == 0 || colActual == ancho - 1) {
        direccionColumna *= -1;
    }

    if (pathActual.indiceActual < pathActual.longitud) {
        int siguiente = pathActual.nodos[pathActual.indiceActual];
        int filaSiguiente = siguiente / ancho;
        int colSiguiente = siguiente % ancho;
        if (filaActual != filaSiguiente) direccionFila *= -1;
        if (colActual != colSiguiente) direccionColumna *= -1;
    }

    int filaTemp = filaActual + direccionFila;
    int colTemp = colActual + direccionColumna;

    // Cambiado: detenerse si encuentra un objeto; si es un Tanque incluirlo como destino
    while (filaTemp >= 0 && filaTemp < largo && colTemp >= 0 && colTemp < ancho) {
        int nodoTemp = filaTemp * ancho + colTemp;
        Node* n = grafo->getNodo(nodoTemp);
        Object* obj = n ? n->getObjeto() : nullptr;
        if (obj != nullptr) {
            if (strcmp(obj->getTipo(), "Tanque") == 0) {
                // incluir el tanque como destino (avanzar una celda más para que luego se reste)
                filaTemp += direccionFila;
                colTemp += direccionColumna;
            }
            break; // obstáculo u otro objeto, parar
        }
        if (!grafo->disponible(nodoTemp)) break;
        filaTemp += direccionFila;
        colTemp += direccionColumna;
    }

    int nuevaFila = filaTemp - direccionFila;
    int nuevaCol = colTemp - direccionColumna;

    if (nuevaFila < 0) nuevaFila = 0;
    if (nuevaFila >= largo) nuevaFila = largo - 1;
    if (nuevaCol < 0) nuevaCol = 0;
    if (nuevaCol >= ancho) nuevaCol = ancho - 1;

    int nuevoDestino = nuevaFila * ancho + nuevaCol;

    if (nuevoDestino == nodoActual) {
        int backFila = filaActual - direccionFila;
        int backCol = colActual - direccionColumna;
        if (backFila >= 0 && backFila < largo && backCol >= 0 && backCol < ancho) {
            int backNodo = backFila * ancho + backCol;
            if (grafo->disponible(backNodo)) {
                nuevoDestino = backNodo;
            }
        }
    }

    if (nuevoDestino == nodoActual) {
        activo = false;
        return;
    }

    // Confirmado que hay rebote válido: consumir un rebote
    rebotes--;

    pathActual = movimientoBala(grafo, nodoActual, nuevoDestino);
    rebotoReciente = true;
}
void Bala::impacto(int nodoImpId) {
    // Limpiar nodo actual
    Node* nodoActualPtr2 = grafo->getNodo(nodoActual);
    if (nodoActualPtr2 != nullptr) {
        Object* objActual = nodoActualPtr2->getObjeto();
        if (objActual != nullptr && strcmp(objActual->getTipo(), "Bala") == 0)
            nodoActualPtr2->setObjeto(nullptr);
    }

    Node* nodoImp = grafo->getNodo(nodoImpId);
    if (nodoImp == nullptr) {
        activo = false;
        return;
    }

    Object* obj = nodoImp->getObjeto();
    if (obj != nullptr && strcmp(obj->getTipo(), "Tanque") == 0) {
        Tanque* tanque = static_cast<Tanque*>(obj);
        if (poderAtaque)
            tanque->recibirdanhototal();
        else
            tanque->recibirdanho();
    }

    activo = false;
}

Path Bala::getPath() const {   
    return pathActual;
}

const char* Bala::getTipo() const {
    return "Bala";
}

bool Bala::getRebotoReciente() const {
    return rebotoReciente;
}

void Bala::setRebotoReciente(bool valor) {
    rebotoReciente = valor;
}