#include "Renderer.h"
#include "Node.h"
#include "Tanque.h"
#include "Obstaculo.h"
#include <iostream>
#include <cstring>
#include <sstream>

Renderer::Renderer(Graph* grafo, Jugador* j1, Jugador* j2)
    : grafo(grafo), jugador1(j1), jugador2(j2), fontLoaded(false),
    turnoTexto(nullptr), infoTanquesTexto(nullptr),
    window(sf::VideoMode({ (unsigned int)(COLS * CELL_SIZE), (unsigned int)(ROWS * CELL_SIZE + 120) }), "Tank Attack") {
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

    hudFondo.setSize(sf::Vector2f(static_cast<float>(COLS * CELL_SIZE), 120.0f));
    hudFondo.setPosition(sf::Vector2f(0.0f, static_cast<float>(ROWS * CELL_SIZE)));
    hudFondo.setFillColor(sf::Color(50, 50, 50, 230));
    hudFondo.setOutlineThickness(2.0f);
    hudFondo.setOutlineColor(sf::Color::White);

    turnoTexto = new sf::Text(font, "", 24);
    infoTanquesTexto = new sf::Text(font, "", 18);
}

void Renderer::updateHUD(Jugador* jugadorActivo) {
    if (!fontLoaded || turnoTexto == nullptr || infoTanquesTexto == nullptr) {
        return;
    }

    std::string turnoStr = "Turno: ";
    turnoStr += jugadorActivo->getNombre();

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
            case 'Z': ss << "Azul "; break;
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
            case 'Z': ss << "Azul "; break;
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
        }
    }

    drawHUD();
    window.display();
}

void Renderer::updateTurnDisplay(Jugador* jugadorActivo) {
    updateHUD(jugadorActivo);
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