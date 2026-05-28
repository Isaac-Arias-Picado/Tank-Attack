#include "Renderer.h"
#include "Node.h"
#include "Tanque.h"
#include "Obstaculo.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include "Cola.h"
#include "Powerup.h"

Renderer::Renderer(Graph* grafo, Jugador* j1, Jugador* j2)
    : grafo(grafo), jugador1(j1), jugador2(j2), fontLoaded(false),
    turnoTexto(nullptr), infoTanquesTexto(nullptr), mostrarRutaBala(false), mostrarRutaTanque(false), nodoTeletransporte(-1),
    tiempoMostrarRuta(1.0f), window(sf::VideoMode({ (unsigned int)(COLS * CELL_SIZE), (unsigned int)(ROWS * CELL_SIZE + 250) }), "Tank Attack") {
    window.setFramerateLimit(60);
    initHUD();
}

Renderer::~Renderer() {
    delete turnoTexto;
    delete infoTanquesTexto;
}

void Renderer::initHUD() {
    std::string fontPaths[] = {
        "arial.ttf",
        "C:\\Windows\\Fonts\\arial.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/System/Library/Fonts/Arial.ttf"
    };

    for (const auto& fontPath : fontPaths) {
        if (font.openFromFile(fontPath)) {
            fontLoaded = true;
            break;
        }
    }

    if (!fontLoaded) {
        std::cerr << "Error: No se pudo cargar ninguna fuente" << std::endl;
        return;
    }

    hudFondo.setSize(sf::Vector2f(static_cast<float>(COLS * CELL_SIZE), 250.0f));
    hudFondo.setPosition(sf::Vector2f(0.0f, static_cast<float>(ROWS * CELL_SIZE)));
    hudFondo.setFillColor(sf::Color(50, 50, 50, 230));
    hudFondo.setOutlineThickness(2.0f);
    hudFondo.setOutlineColor(sf::Color::White);

    turnoTexto = new sf::Text(font, "", 24);
    infoTanquesTexto = new sf::Text(font, "", 18);
}

void Renderer::updateHUD(Jugador* jugadorActivo, float tiempoRestante) {
    if (!fontLoaded || turnoTexto == nullptr || infoTanquesTexto == nullptr) {
        return;
    }

    int minutos = (int)tiempoRestante / 60;
    int segundos = (int)tiempoRestante % 60;

    std::string turnoStr = "Turno: ";
    turnoStr += jugadorActivo->getNombre();
    turnoStr += "   Tiempo: ";
    turnoStr += std::to_string(minutos) + ":" + (segundos < 10 ? "0" : "") + std::to_string(segundos);

    *turnoTexto = sf::Text(font, turnoStr, 24);
    turnoTexto->setPosition(sf::Vector2f(10.0f, static_cast<float>(ROWS * CELL_SIZE + 10)));

    if (jugadorActivo->getId() == 0) {
        turnoTexto->setFillColor(sf::Color(100, 200, 255));
    }
    else {
        turnoTexto->setFillColor(sf::Color(255, 100, 100));
    }

    std::stringstream ss;
    ss << "Colores de tanques:\n";
    ss << jugador1->getNombre() << ": ";

    for (int i = 0; i < 4; i++) {
        Tanque* t = jugador1->getTanque(i);
        if (t != nullptr) {
            char color = t->getColor();
            switch (color) {
            case 'A': ss << "Amarillo "; break;
            case 'C': ss << "Cyan "; break;
            case 'Z': ss << "Morado "; break;
            case 'R': ss << "Rojo "; break;
            default: ss << "? "; break;
            }
        }
    }

    ss << "\n" << jugador2->getNombre() << ": ";
    for (int i = 0; i < 4; i++) {
        Tanque* t = jugador2->getTanque(i);
        if (t != nullptr) {
            char color = t->getColor();
            switch (color) {
            case 'A': ss << "Amarillo "; break;
            case 'C': ss << "Cyan "; break;
            case 'Z': ss << "Morado "; break;
            case 'R': ss << "Rojo "; break;
            default: ss << "? "; break;
            }
        }
    }

    *infoTanquesTexto = sf::Text(font, ss.str(), 18);
    infoTanquesTexto->setFillColor(sf::Color::White);
    infoTanquesTexto->setPosition(sf::Vector2f(10.0f, static_cast<float>(ROWS * CELL_SIZE + 50)));
}

void Renderer::drawHUD() {
    if (!fontLoaded || turnoTexto == nullptr || infoTanquesTexto == nullptr) return;

    window.draw(hudFondo);
    window.draw(*turnoTexto);
    window.draw(*infoTanquesTexto);

    float baseY = static_cast<float>(ROWS * CELL_SIZE);
    drawPowerups(jugador1, baseY + 120);
    drawPowerups(jugador2, baseY + 175);
}

void Renderer::drawPowerups(Jugador* jugador, float yPos) {
    if (!fontLoaded) return;

    sf::Text label(font, "PowerUps: ", 16);
    label.setFillColor(sf::Color::White);
    label.setPosition(sf::Vector2f(10.0f, yPos));
    window.draw(label);

    float xPos = 110.0f;
    for (int i = 0; i < jugador->getCola()->getCantNodos(); i++) {
        Powerup* p = jugador->getCola()->getPowerup(i);
        if (p == nullptr) continue;

        sf::Color color = sf::Color::White;
        std::string abrev = "?";
        if (strcmp(p->getNombre(), "DobleTurno") == 0) { color = sf::Color::Blue; abrev = "DT"; }
        else if (strcmp(p->getNombre(), "PrecisionMovimiento") == 0) { color = sf::Color::Green; abrev = "PM"; }
        else if (strcmp(p->getNombre(), "PrecisionAtaque") == 0) { color = sf::Color(255, 165, 0); abrev = "PA"; }
        else if (strcmp(p->getNombre(), "PoderAtaque") == 0) { color = sf::Color::Red; abrev = "PO"; }

        sf::RectangleShape rect(sf::Vector2f(40, 22));
        rect.setFillColor(color);
        rect.setPosition(sf::Vector2f(xPos, yPos));
        window.draw(rect);

        sf::Text texto(font, abrev, 14);
        texto.setFillColor(sf::Color::White);
        texto.setPosition(sf::Vector2f(xPos + 5, yPos + 3));
        window.draw(texto);

        xPos += 45.0f;
    }
}

std::optional<sf::Event> Renderer::pollEvent() {
    return window.pollEvent();
}

void Renderer::close() {
    window.close();
}

bool Renderer::init() {
    if (!texturePiso.loadFromFile("tileSand1.png")) {
        std::cerr << "Error cargando tileSand1.png" << std::endl;
        return false;
    }
    if (!textureCrate.loadFromFile("crateMetal.png")) {
        std::cerr << "Error cargando crateMetal.png" << std::endl;
        return false;
    }
    if (!textureTank1.loadFromFile("tank1-export1.png")) {
        std::cerr << "Error cargando tank1-export1.png" << std::endl;
        return false;
    }
    if (!textureTank2.loadFromFile("tank2-export1.png")) {
        std::cerr << "Error cargando tank2-export1.png" << std::endl;
        return false;
    }
    if (!textureTank3.loadFromFile("tank3-export1.png")) {
        std::cerr << "Error cargando tank3-export1.png" << std::endl;
        return false;
    }
    if (!textureTank4.loadFromFile("tank4-export1.png")) {
        std::cerr << "Error cargando tank4-export1.png" << std::endl;
        return false;
    }
    if (!textureBala.loadFromFile("bala.png")) {
        std::cerr << "Error cargando bala.png" << std::endl;
        return false;
    }
    return true;
}

void Renderer::render() {
    window.clear(sf::Color::Black);
    if ((mostrarRutaTanque || mostrarRutaBala) && relojRuta.getElapsedTime().asSeconds() > tiempoMostrarRuta) {
        limpiarRutas();
    }

    int n = grafo->getN();
    int ancho = grafo->getAncho();

    for (int id = 0; id < n; id++) {
        int fila = id / ancho;
        int col = id % ancho;
        float x = (float)(col * CELL_SIZE);
        float y = (float)(fila * CELL_SIZE);

        Node* nodo = grafo->getNodo(id);
        Object* obj = nodo->getObjeto();

        sf::Sprite spritePiso(texturePiso);
        sf::Vector2u sizePiso = texturePiso.getSize();
        spritePiso.setScale(sf::Vector2f((float)CELL_SIZE / sizePiso.x, (float)CELL_SIZE / sizePiso.y));
        spritePiso.setPosition(sf::Vector2f(x, y));
        window.draw(spritePiso);

        if (obj == nullptr) continue;

        const char* tipo = obj->getTipo();
        if (tipo == nullptr) continue;

        if (strcmp(tipo, "Obstaculo") == 0) {
            sf::Sprite spriteCrate(textureCrate);
            sf::Vector2u size = textureCrate.getSize();
            spriteCrate.setScale(sf::Vector2f((float)CELL_SIZE / size.x, (float)CELL_SIZE / size.y));
            spriteCrate.setPosition(sf::Vector2f(x, y));
            window.draw(spriteCrate);
        }
        else if (strcmp(tipo, "Bala") == 0) {
            sf::Sprite spriteBala(textureBala);
            sf::Vector2u size = textureBala.getSize();
            spriteBala.setScale(sf::Vector2f((float)CELL_SIZE / size.x, (float)CELL_SIZE / size.y));
            spriteBala.setPosition(sf::Vector2f(x, y));
            window.draw(spriteBala);
        }
        else if (strcmp(tipo, "Tanque") == 0) {
            Tanque* t = static_cast<Tanque*>(obj);
            char color = t->getColor();

            sf::Texture* tex = nullptr;
            if (color == 'A') tex = &textureTank1;
            else if (color == 'C') tex = &textureTank2;
            else if (color == 'Z') tex = &textureTank3;
            else if (color == 'R') tex = &textureTank4;

            if (tex) {
                sf::Sprite spriteTanque(*tex);
                sf::Vector2u size = tex->getSize();
                spriteTanque.setScale(sf::Vector2f((float)CELL_SIZE / size.x, (float)CELL_SIZE / size.y));
                spriteTanque.setPosition(sf::Vector2f(x, y));
                window.draw(spriteTanque);
            }

            // Barra de vida
            float pct = t->getVida() / t->getVidaMaxima();
            float barW = (float)CELL_SIZE - 4.f;
            float barH = 5.f;
            float barX = x + 2.f;
            float barY = y + CELL_SIZE - barH - 2.f;

            sf::RectangleShape fondo(sf::Vector2f(barW, barH));
            fondo.setPosition(sf::Vector2f(barX, barY));
            fondo.setFillColor(sf::Color(80, 0, 0));
            window.draw(fondo);

            sf::RectangleShape relleno(sf::Vector2f(barW * pct, barH));
            relleno.setPosition(sf::Vector2f(barX, barY));
            relleno.setFillColor(pct > 0.5f ? sf::Color(0, 200, 0) : sf::Color(200, 0, 0));
            window.draw(relleno);
        }
    }

    // Dibujar ruta del tanque
    if (mostrarRutaTanque) {
        for (int i = 0; i < rutaTanque.longitud; i++) {
            int id = rutaTanque.nodos[i];
            int fila = id / ancho;
            int col = id % ancho;
            sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            rect.setPosition(sf::Vector2f(col * CELL_SIZE, fila * CELL_SIZE));
            rect.setFillColor(sf::Color(255, 255, 0, 80)); //amarillo 
            window.draw(rect);
        }
    }
    // Dibujar nodo de teletransporte
    if (nodoTeletransporte != -1) {
        int fila = nodoTeletransporte / ancho;
        int col = nodoTeletransporte % ancho;
        sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        rect.setPosition(sf::Vector2f(col * CELL_SIZE, fila * CELL_SIZE));
        rect.setFillColor(sf::Color(0, 100, 255, 120));
        window.draw(rect);
    }
    // Dibujar ruta de la bala
    if (mostrarRutaBala) {
        for (int i = 0; i < rutaBala.longitud; i++) {
            int id = rutaBala.nodos[i];
            int fila = id / ancho;
            int col = id % ancho;
            sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            rect.setPosition(sf::Vector2f(col * CELL_SIZE, fila * CELL_SIZE));
            rect.setFillColor(sf::Color(255, 0, 0, 80)); //rojo 
            window.draw(rect);
        }
    }

    drawHUD();
    window.display();
}

void Renderer::mostrarVictoria(Jugador* ganador) {
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>()) {
                auto* kp = event->getIf<sf::Event::KeyPressed>();
                if (kp->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        window.clear(sf::Color(30, 30, 60)); 

        // fondo del mensaje
        sf::RectangleShape fondo(sf::Vector2f(600.f, 200.f));
        fondo.setFillColor(sf::Color(0, 0, 0, 180));
        fondo.setOutlineThickness(3.f);
        fondo.setOutlineColor(sf::Color::White);
        fondo.setPosition(sf::Vector2f(
            (COLS * CELL_SIZE - 600.f) / 2.f,
            (ROWS * CELL_SIZE - 200.f) / 2.f
        ));
        window.draw(fondo);

        // texto principal
        std::string msg;
        if (ganador == nullptr)
            msg = "Empate!";
        else {
            msg = "Gano: ";
            msg += ganador->getNombre();
        }

        sf::Text texto(font, msg, 48);
        texto.setFillColor(ganador == nullptr ? sf::Color::White : sf::Color::Yellow);
        sf::FloatRect bounds = texto.getLocalBounds();
        texto.setPosition(sf::Vector2f(
            (COLS * CELL_SIZE - bounds.size.x) / 2.f,
            (ROWS * CELL_SIZE - bounds.size.y) / 2.f - 20.f
        ));
        window.draw(texto);

        // texto instruccion
        sf::Text instruccion(font, "Presiona ESC para salir", 24);
        instruccion.setFillColor(sf::Color(200, 200, 200));
        sf::FloatRect bounds2 = instruccion.getLocalBounds();
        instruccion.setPosition(sf::Vector2f(
            (COLS * CELL_SIZE - bounds2.size.x) / 2.f,
            (ROWS * CELL_SIZE - bounds2.size.y) / 2.f + 50.f
        ));
        window.draw(instruccion);

        window.display();
    }
}

bool Renderer::isOpen() const {
    return window.isOpen();
}

void Renderer::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
    }

}

void Renderer::setRutaTanque(Path p) {
    rutaTanque = p;
    mostrarRutaTanque = true;
    relojRuta.restart();
}

void Renderer::setRutaBala(Path p) {
    rutaBala = p;
    mostrarRutaBala = true;
    relojRuta.restart();
}

void Renderer::limpiarRutas() {
    mostrarRutaTanque = false;
    mostrarRutaBala = false;
    nodoTeletransporte = -1;
}

void Renderer::setNodoTeleporte(int nodo) {
    nodoTeletransporte = nodo;
}

void Renderer::limpiarNodoTeleporte() {
    nodoTeletransporte = -1;
}