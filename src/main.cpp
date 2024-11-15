#include <SFML/Graphics.hpp>

import board;

int main() {
    sf::RenderWindow window({800, 800}, "DameCplusplus", sf::Style::Default);
    window.setFramerateLimit(144);

    sf::View fixedView(sf::FloatRect(0, 0, 800, 800));

    board::GameBoard gameBoard;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {
                float minSize = std::min(event.size.width, event.size.height);

                float xOffset = (event.size.width - minSize) / 2.0f;
                float yOffset = (event.size.height - minSize) / 2.0f;

                fixedView.setViewport(sf::FloatRect(
                    xOffset / event.size.width,
                    yOffset / event.size.height,
                    minSize / event.size.width,
                    minSize / event.size.height
                ));

                window.setView(fixedView);
            }
        }

        window.clear(sf::Color(50, 50, 50));
        gameBoard.draw(window);
        window.display();
    }

    return 0;
}