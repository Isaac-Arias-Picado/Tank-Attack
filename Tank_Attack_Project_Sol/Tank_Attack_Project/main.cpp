#include <SFML/Graphics.hpp>
#include <optional>

int main() {
    sf::VideoMode mode({ 800, 600 });
    sf::RenderWindow window(mode, "Prueba SFML");

    sf::CircleShape shape(50.f);
    shape.setFillColor(sf::Color::Cyan);
    shape.setPosition({ 375.f, 275.f });

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}