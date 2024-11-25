#include <iostream>
#include <SFML/Graphics.hpp>

import board;
import position;
import windowManager;
import game;

int main() {
    sf::RenderWindow window({1920, 1080}, "DameCplusplus", sf::Style::Default);
    window.setFramerateLimit(144);

    auto views = windowManager::handleResize(window);
    board::GameBoard gameBoard(&window);

    position::Position selectedPos(-1, -1);

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
                views = windowManager::handleResize(window);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                window.setView(views.gameView);
                sf::Vector2f worldPos = windowManager::getMouseBoardPosition(window);
                Game::handleClick(worldPos, selectedPos, gameBoard, window);
            }
        }

        window.clear(sf::Color(50, 50, 50));

        window.setView(views.gameView);
        gameBoard.draw(window, selectedPos);

        window.setView(views.guiView);
        gameBoard.drawGameInfo(window, gameBoard, font, views);

        window.display();
    }

    return 0;
}