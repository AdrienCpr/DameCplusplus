module;

#include <SFML/Graphics.hpp>

export module windowManager;

export namespace windowManager {
    struct ViewInfo {
        sf::View gameView;
        sf::View guiView;
        sf::Vector2f boardPosition;
        float boardSize;
    };

    ViewInfo handleResize(sf::RenderWindow& window);
    sf::Vector2f getMouseBoardPosition(const sf::RenderWindow& window);
}
