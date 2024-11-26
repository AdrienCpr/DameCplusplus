#include <iostream>
#include <SFML/Graphics.hpp>

import game;
import windowManager;

int main() {
    sf::RenderWindow window({1920, 1080}, "DameCplusplus", sf::Style::Fullscreen);
    window.setFramerateLimit(144);

    auto views = windowManager::handleResize(window);

    try {
        Game::GameState gameState(&window);

        while (window.isOpen()) {
            gameState.handleEvents(window, views);
            gameState.update(window);
            gameState.render(window, views);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}