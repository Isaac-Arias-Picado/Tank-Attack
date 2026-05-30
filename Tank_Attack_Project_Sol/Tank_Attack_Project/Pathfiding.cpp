#include "Pathfinding.h"
#include "Cola_Prioridad.h"
#include "Cola.h"
#include <cstdlib>
#include <cmath>
#include <cstring>
#include "Node.h"
#include <iostream>


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
    path.indiceActual = 1; 

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

    if (origen == destino) return true;

    int dx = abs(colD - colO);
    int dy = abs(filaD - filaO);
    int pasoX = (colD > colO) ? 1 : -1;
    int pasoY = (filaD > filaO) ? 1 : -1;

    int error = dx - dy;
    int x = colO, y = filaO;

    while (true) {
        if (x == colD && y == filaD) return true;

        // Verificar celda actual
        int celdaActual = y * ancho + x;
        if (celdaActual != origen && !grafo->disponible(celdaActual)) {
            return false;
        }

        // Para diagonales verificar también la celda 
        int error2 = 2 * error;
        bool moverX = (error2 > -dy);
        bool moverY = (error2 < dx);

        if (moverX && moverY) {
            int celdaLateralX = y * ancho + (x + pasoX);
            int celdaLateralY = (y + pasoY) * ancho + x;

            if (celdaLateralX != origen && !grafo->disponible(celdaLateralX)) {
                return false;
            }
            if (celdaLateralY != origen && !grafo->disponible(celdaLateralY)) {
                return false;
            }
        }

        if (moverX) {
            error -= dy;
            x += pasoX;
        }
        if (moverY) {
            error += dx;
            y += pasoY;
        }

        if (x < 0 || x >= ancho || y < 0 || y >= (400 / ancho)) {
            return false;
        }
    }
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
    return origen; 
}
static Path avanzarHastaDonde(Graph* grafo, int origen, int destino, int ancho) {
    int filaActual = origen / ancho, colActual = origen % ancho;
    int filaD = destino / ancho, colD = destino % ancho;

    int dc = abs(colD - colActual);
    int df = abs(filaD - filaActual);
    int movCol = (colD > colActual) ? 1 : -1;
    int movFila = (filaD > filaActual) ? 1 : -1;
    int error = dc - df;

    Path path;
    int cont = 0;

    while (true) {
        int id = filaActual * ancho + colActual;

        if (cont > 0 && !grafo->disponible(id)) break;

        path.nodos[cont++] = id;

        if (filaActual == filaD && colActual == colD) break;

        if (error > 0) {
            colActual += movCol;
            error -= df;
        }
        else if (error < 0) {
            filaActual += movFila;
            error += dc;
        }
        else {
            colActual += movCol;
            error -= df;
        }
    }

    path.longitud = cont;
    path.indiceActual = 1;
    return path;
}
Path movimientoAleatorio(Graph* grafo, int origen, int destino, int radio) {
    int ancho = grafo->getAncho();
    int largo = grafo->getLargo();

    //linea vista directa origen 
    if (lineaVista(grafo, origen, destino, ancho)) {
        return avanzarHastaDonde(grafo, origen, destino, ancho);
    }

    // Teletransporte a posicion aleatoria en el radio
    int intermedio = nodoAleatorioEnRadio(grafo, origen, radio, ancho, largo);
    if (intermedio == origen) {
        // No encontro intermedio caminar desde origen hasta donde se pueda
        return avanzarHastaDonde(grafo, origen, destino, ancho);
    }

    // Desde el intermedio, caminar en escalera hacia destino
    Path pathFinal = avanzarHastaDonde(grafo, intermedio, destino, ancho);

    // El tanque aparece en intermedio y camina de ahi al destino
    Path resultado;
    resultado.nodos[resultado.longitud++] = origen;    // posicion inicial
    resultado.nodos[resultado.longitud++] = intermedio; // salto instantaneo
    for (int i = 1; i < pathFinal.longitud; i++)
        resultado.nodos[resultado.longitud++] = pathFinal.nodos[i];
    resultado.indiceActual = 1;
    resultado.fueBloqueado = true;
    resultado.nodoTeletransporte = intermedio;
    return resultado;
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
            error -= dify;
        }
    }

    path.longitud = cont;
    path.indiceActual = 1;
    return path;  
}