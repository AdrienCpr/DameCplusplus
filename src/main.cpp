#include <iostream>
#include <SFML/Graphics.hpp>

import board;
import position;
import windowManager;
import game;

int main() {
    sf::RenderWindow window({800, 800}, "DameCplusplus", sf::Style::Default);
    window.setFramerateLimit(144);

    sf::View fixedView(sf::FloatRect(0, 0, 800, 800));
    board::GameBoard gameBoard;

    position::Position selectedPos(-1, -1);

    windowManager::handleResize(window, fixedView);

    sf::Font font;
    if (!font.loadFromFile("resources/fonts/arial.ttf")) {
        std::cerr << "Erreur : Impossible de charger la police." << std::endl;
        return 1;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                windowManager::handleResize(window, fixedView);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                Game::handleClick(event, selectedPos, gameBoard, window); // Passer 'window'
            }
        }

        window.clear(sf::Color(50, 50, 50));
        gameBoard.draw(window, selectedPos);
        gameBoard.drawGameInfo(window, gameBoard, font);
        window.display();
    }

    return 0;
}
