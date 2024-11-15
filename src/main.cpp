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

    position::Position selectedPos(-1, -1); // Position sélectionnée, initialement invalide

    // Gérer la logique de redimensionnement de la fenêtre
    windowManager::handleResize(window, fixedView);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Gérer l'événement de redimensionnement de la fenêtre
            else if (event.type == sf::Event::Resized) {
                windowManager::handleResize(window, fixedView);
            }

            // Gérer les clics de souris et les mouvements
            if (event.type == sf::Event::MouseButtonPressed) {
                Game::handleClick(event, selectedPos, gameBoard);
            }
        }

        // Nettoyer et redessiner la fenêtre
        window.clear(sf::Color(50, 50, 50));
        gameBoard.draw(window);
        window.display();
    }

    return 0;
}