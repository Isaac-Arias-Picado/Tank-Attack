#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "Graph.h"
#include "Jugador.h"
#include "Bala.h"

#define CELL_SIZE 64
#define ROWS 20
#define COLS 20

class Renderer {
private:
    Graph* grafo;
    Jugador* jugador1;
    Jugador* jugador2;
    sf::RenderWindow window;

    sf::Texture texturePiso;
    sf::Texture textureCrate;
    sf::Texture textureTank1;
    sf::Texture textureTank2;
    sf::Texture textureTank3;
    sf::Texture textureTank4;
    sf::Texture textureBala;

    sf::Font font;
    bool fontLoaded;
    sf::Text* turnoTexto;
    sf::Text* infoTanquesTexto;
    sf::RectangleShape hudFondo;

    Path rutaTanque;
    Path rutaBala;
    bool mostrarRutaTanque;
    bool mostrarRutaBala;
    int nodoTeletransporte;

    void initHUD();
    void drawHUD();

    std::string ultimoPowerupJ1;
    std::string ultimoPowerupJ2;

public:
    Renderer(Graph* grafo, Jugador* j1, Jugador* j2);
    ~Renderer();

    bool init();
    void render();
    void updateHUD(Jugador* jugadorActivo, float tiempoRestante);
    std::optional<sf::Event> pollEvent();
    void close();
    bool isOpen() const;
    void handleEvents();

    sf::RenderWindow& getWindow() { return window; }
    sf::Vector2u getWindowSize() const { return window.getSize(); }

    void setRutaTanque(Path p);
    void setRutaBala(Path p);
    void limpiarRutas();
    void drawPowerups(Jugador* jugador, float yPos);
    void mostrarVictoria(Jugador* ganador);
    void setNodoTeleporte(int nodo);
    void limpiarNodoTeleporte();
    void setUltimoPowerup(int jugadorId, const char* nombre);
    void limpiarUltimoPowerup(int jugadorId);
};