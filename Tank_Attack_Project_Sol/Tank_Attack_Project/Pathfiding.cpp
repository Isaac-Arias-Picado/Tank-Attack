#include "Pathfinding.h"
#include "Cola_Prioridad.h"
#include "Cola.h"
#include <cstdlib>
#include <cmath>
#include <cstring>
#include "Node.h"


Path dijkstra(Graph* grafo, int origen, int destino) {
    int n = grafo->getN(); 
    float dist[400];
    int prev[400];
    bool visitado[400];

    for (int i = 0; i < n; i++) {
        dist[i] = 1e9f;
        prev[i] = -1;
        visitado[i] = false;
    }

    dist[origen] = 0.0f;
    ColaPrioridad pq;
    pq.Insertar(origen, 0.0f);

    while (!pq.vacio()) {
        NodoHeap actual = pq.Sacarmin();
        int u = actual.idnodo;

        if (u < 0 || u >= n) continue;
        if (visitado[u]) continue;
        visitado[u] = true;

        if (u == destino) break;

        for (int v = 0; v < n; v++) {
            if (!grafo->getMatriz()[u][v]) continue;
            if (!grafo->disponible(v) && v != destino) continue;
            if (visitado[v]) continue;

            float nuevaDist = dist[u] + 1.0f;
            if (nuevaDist < dist[v]) {
                dist[v] = nuevaDist;
                prev[v] = u;
                pq.Insertar(v, nuevaDist);
            }
        }
    }

    // Reconstruir path
    Path path;
    if (prev[destino] == -1 && destino != origen) return path; // sin camino

    int temp[400];
    int count = 0;
    int cur = destino;
    while (cur != -1) {
        temp[count++] = cur;
        cur = prev[cur];
    }

    // Invertir
    for (int i = 0; i < count; i++) {
        path.nodos[i] = temp[count - 1 - i];
    }
    path.longitud = count;
    path.indiceActual = 1; // 0 es origen, empezamos en 1

    return path;
}

int heuristica(int origen, int destino, int ancho) {
    int filaOrigen = origen / ancho;
    int filaDestino = destino / ancho;
    int columnaOrigen = origen % ancho;
    int columnaDestino = destino % ancho;
    return abs((filaDestino - filaOrigen)) + abs((columnaDestino - columnaOrigen));
}

Path AAsterisco(Graph* grafo, int origen, int destino) {
    int ancho = grafo->getAncho();
    int n = grafo->getN();
    float dist[400];
    int prev[400];
    bool visitado[400];

    for (int i = 0; i < n; i++) {
        dist[i] = 1e9f;
        prev[i] = -1;
        visitado[i] = false;
    }

    dist[origen] = 0.0f;
    ColaPrioridad pq;
    pq.Insertar(origen, 0.0f);

    while (!pq.vacio()) {
        NodoHeap actual = pq.Sacarmin();
        int u = actual.idnodo;

        if (u < 0 || u >= n) continue;
        if (visitado[u]) continue;
        visitado[u] = true;

        if (u == destino) break;

        for (int v = 0; v < n; v++) {
            if (!grafo->getMatriz()[u][v]) continue;
            if (!grafo->disponible(v) && v != destino) continue;
            if (visitado[v]) continue;

            float nuevaDist = dist[u] + 1.0f;
            if (nuevaDist < dist[v]) {
                dist[v] = nuevaDist;
                prev[v] = u;
                pq.Insertar(v, nuevaDist+heuristica(v,destino,ancho));
            }
        }
    }

    // Reconstruir path
    Path path;
    if (prev[destino] == -1 && destino != origen) return path; // sin camino

    int temp[400];
    int count = 0;
    int cur = destino;
    while (cur != -1) {
        temp[count++] = cur;
        cur = prev[cur];
    }

    // Invertir
    for (int i = 0; i < count; i++) {
        path.nodos[i] = temp[count - 1 - i];
    }
    path.longitud = count;
    path.indiceActual = 1; // 0 es origen, empezamos en 1

    return path;
}

Path bfs(Graph* grafo, int origen, int destino) {
    int n = grafo->getN();
    int prev[400];
    bool visitado[400];

    for (int i = 0; i < n; i++) {
        prev[i] = -1;
        visitado[i] = false;
    }

    // Array circular como queue
    int queue[400];
    int frente = 0, fin = 0;

    visitado[origen] = true;
    queue[fin++] = origen;

    while (frente != fin) {
        int u = queue[frente++];

        if (u == destino) break;

        for (int v = 0; v < n; v++) {
            if (!grafo->getMatriz()[u][v]) continue;
            if (!grafo->disponible(v) && v != destino) continue;
            if (visitado[v]) continue;

            visitado[v] = true;
            prev[v] = u;
            queue[fin++] = v;
        }
    }

    // Reconstruir path
    Path path;
    if (prev[destino] == -1 && destino != origen) return path;

    int temp[400];
    int count = 0;
    int cur = destino;
    while (cur != -1) {
        temp[count++] = cur;
        cur = prev[cur];
    }

    for (int i = 0; i < count; i++) {
        path.nodos[i] = temp[count - 1 - i];
    }
    path.longitud = count;
    path.indiceActual = 1;

    return path;
}

static bool lineaVista(Graph* grafo, int origen, int destino, int ancho) {
    int filaO = origen / ancho, colO = origen % ancho;
    int filaD = destino / ancho, colD = destino % ancho;

    // Solo funciona en linea recta ortogonal
    if (filaO != filaD && colO != colD) return false;

    if (filaO == filaD) {
        int minCol = (colO < colD) ? colO : colD;
        int maxCol = (colO > colD) ? colO : colD;
        for (int c = minCol + 1; c < maxCol; c++) {
            int nodo = filaO * ancho + c;
            if (!grafo->disponible(nodo)) return false;
        }
    }
    else {
        int minFila = (filaO < filaD) ? filaO : filaD;
        int maxFila = (filaO > filaD) ? filaO : filaD;
        for (int f = minFila + 1; f < maxFila; f++) {
            int nodo = f * ancho + colO;
            if (!grafo->disponible(nodo)) return false;
        }
    }
    return true;
}

static int nodoAleatorioEnRadio(Graph* grafo, int origen, int radio, int ancho, int largo) {
    int fila = origen / ancho;
    int col = origen % ancho;

    // Intentar hasta 10 veces encontrar nodo disponible en el radio
    for (int intento = 0; intento < 10; intento++) {
        int df = (rand() % (2 * radio + 1)) - radio;
        int dc = (rand() % (2 * radio + 1)) - radio;
        int nuevaFila = fila + df;
        int nuevaCol = col + dc;

        if (nuevaFila < 0 || nuevaFila >= largo) continue;
        if (nuevaCol < 0 || nuevaCol >= ancho) continue;

        int nodo = nuevaFila * ancho + nuevaCol;
        if (grafo->disponible(nodo)) return nodo;
    }
    return origen; // no encontro nada
}

static Path avanzarHastaDonde(Graph* grafo, int origen, int destino, int ancho) {
    int filaO = origen / ancho, colO = origen % ancho;
    int filaD = destino / ancho, colD = destino % ancho;

    int ultimo = origen;

    if (filaO == filaD) {
        int paso = (colD > colO) ? 1 : -1;
        for (int c = colO + paso; c != colD + paso; c += paso) {
            int nodo = filaO * ancho + c;
            if (!grafo->disponible(nodo)) break;
            ultimo = nodo;
        }
    }
    else if (colO == colD) {
        int paso = (filaD > filaO) ? 1 : -1;
        for (int f = filaO + paso; f != filaD + paso; f += paso) {
            int nodo = f * ancho + colO;
            if (!grafo->disponible(nodo)) break;
            ultimo = nodo;
        }
    }

    // Construir path directo de origen a ultimo
    Path path;
    if (ultimo == origen) return path;

    int filaU = ultimo / ancho, colU = ultimo % ancho;
    int count = 0;
    if (filaO == filaU) {
        int paso = (colU > colO) ? 1 : -1;
        for (int c = colO; c != colU + paso; c += paso)
            path.nodos[count++] = filaO * ancho + c;
    }
    else {
        int paso = (filaU > filaO) ? 1 : -1;
        for (int f = filaO; f != filaU + paso; f += paso)
            path.nodos[count++] = f * ancho + colO;
    }
    path.longitud = count;
    path.indiceActual = 1;
    return path;
}

Path movimientoAleatorio(Graph* grafo, int origen, int destino, int radio) {
    int ancho = grafo->getAncho();
    int largo = grafo->getLargo();

    // Intento 1: linea vista directa
    if (lineaVista(grafo, origen, destino, ancho)) {
        return avanzarHastaDonde(grafo, origen, destino, ancho);
    }

    // Intento 2: moverse a nodo aleatorio, luego linea vista de nuevo
    int intermedio = nodoAleatorioEnRadio(grafo, origen, radio, ancho, largo);
    if (intermedio != origen) {
        // Path: origen -> intermedio
        Path pathIntermedio = avanzarHastaDonde(grafo, origen, intermedio, ancho);

        if (lineaVista(grafo, intermedio, destino, ancho)) {
            // Concatenar intermedio -> destino
            Path pathFinal = avanzarHastaDonde(grafo, intermedio, destino, ancho);
            // Unir los dos paths
            Path resultado;
            // primero los nodos de pathIntermedio
            for (int i = 0; i < pathIntermedio.longitud; i++)
                resultado.nodos[resultado.longitud++] = pathIntermedio.nodos[i];
            // luego pathFinal desde indice 1 
            for (int i = 1; i < pathFinal.longitud; i++)
                resultado.nodos[resultado.longitud++] = pathFinal.nodos[i];
            resultado.indiceActual = 1;
            return resultado;
        }

        // Intento 2 fallo tambien: avanzar hasta donde se pueda desde intermedio
        if (pathIntermedio.longitud > 0) return pathIntermedio;
    }

    // Ultimo recurso: avanzar hasta donde se pueda desde origen
    return avanzarHastaDonde(grafo, origen, destino, ancho);
}

Path movimientoBala(Graph* grafo, int origen, int destino) {
    int ancho = grafo->getAncho();
    int filaOrigen = origen / ancho;
    int colOrigen = origen % ancho;
    int filaDestino = destino / ancho;
    int colDestino = destino % ancho;

    int difx = abs(colDestino - colOrigen);
    int dify = abs(filaDestino - filaOrigen);
    int error = difx - dify;

    int movCol;
    if (colDestino > colOrigen) {
        movCol = 1;
    }
    else {
        movCol = -1;
    }

    int movFila;
    if (filaDestino > filaOrigen) {
        movFila = 1;
    }
    else {
        movFila = -1;
    }

    Path path;
    int cont = 0;
    while (true) {
        int id = filaOrigen * ancho + colOrigen;
        path.nodos[cont++] = id;

        if (filaOrigen == filaDestino && colOrigen == colDestino) break;  // llegamos al destino
        Node* n = grafo->getNodo(id);
        Object* obj = nullptr;
        if (n != nullptr) {
            obj = n->getObjeto();
        }
        if (obj != nullptr) {
            if (strcmp(obj->getTipo(), "Tanque") == 0) break;   // parar en tanque
            if (strcmp(obj->getTipo(), "Obstaculo") == 0) break; // parar en obstáculo
        }
        if (error > 0) {
            colOrigen += movCol;
            error -= dify;
        }
        else if (error < 0) {
            filaOrigen += movFila;
            error += difx;
        }
        else {
            colOrigen += movCol;
            filaOrigen += movFila;
            error += difx - dify;
        }
    }

    path.longitud = cont;
    path.indiceActual = 1;
    return path;  
}