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
    std::cout << "origen fila=" << origen / ancho << " col=" << origen % ancho << std::endl;
    std::cout << "destino fila=" << destino / ancho << " col=" << destino % ancho << std::endl;
    if (origen == destino) {
        return false;
    }

    if ( (filaO == filaD) && (colO!= colD)) {
        std::cout << "Entro" << std::endl;
        bool libreH = true;
        int c = colO;
        int paso = (colD > colO) ? 1 : -1;
        for (c = colO + paso; c != colD + paso; c += paso) {
            if (!grafo->disponible(filaO * ancho + c)) { libreH = false; break; }
        }
        if (libreH) {
            std::cout << "Aplicando horizontal" << std::endl;
            int f;
            int pasoF = (filaD > filaO) ? 1 : -1;
            for (f = filaO + pasoF; f != filaD + pasoF; f += pasoF) {
                if (!grafo->disponible(f * ancho + colD)) { libreH = false; break; }
            }
        }
        if (libreH) return true;
    }

    if ((filaO != filaD) && (colO == colD)) {
        bool libreV = true;
        int pasoF = (filaD > filaO) ? 1 : -1;
        int c = filaO;
        int f;
        for (f = filaO + pasoF; f != filaD + pasoF; f += pasoF) {
            if (!grafo->disponible(f * ancho + colO)) { libreV = false; break; }
        }
        if (libreV) {
            std::cout << "Aplicando vertical" << std::endl;
            int pasoC = (colD > colO) ? 1 : -1;
            for (c = colO + pasoC; c != colD + pasoC; c += pasoC) {
                if (!grafo->disponible(filaD * ancho + c)) { libreV = false; break; }
            }
        }
        return libreV;
    }

    // Ortogonal
    if (filaO == filaD || colO == colD) {
        if (filaO == filaD) {
            int min = (colO < colD) ? colO : colD;
            int max = (colO > colD) ? colO : colD;
            for (int c = min + 1; c < max; c++)
                if (!grafo->disponible(filaO * ancho + c)) return false;
        }
        else {
            int min = (filaO < filaD) ? filaO : filaD;
            int max = (filaO > filaD) ? filaO : filaD;
            for (int f = min + 1; f < max; f++)
                if (!grafo->disponible(f * ancho + colO)) return false;
        }
        return true;
    }
    return false;
}


static int nodoAleatorioEnRadio(Graph* grafo, int origen, int radio, int ancho, int largo) {
    std::cout << "Nodo teletrnasportandose" << std::endl;
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

    // Intento 1: linea vista directa origen -> destino
    if (lineaVista(grafo, origen, destino, ancho)) {
        return avanzarHastaDonde(grafo, origen, destino, ancho);
    }

    // Teletransporte a posicion aleatoria en el radio
    int intermedio = nodoAleatorioEnRadio(grafo, origen, radio, ancho, largo);
    if (intermedio == origen) {
        // No encontro intermedio, caminar desde origen hasta donde se pueda
        return avanzarHastaDonde(grafo, origen, destino, ancho);
    }

    // Desde el intermedio, caminar en escalera hacia destino
    Path pathFinal = avanzarHastaDonde(grafo, intermedio, destino, ancho);

    // Path: origen (nodo actual) + camino desde intermedio
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