module;

#include <SFML/Graphics.hpp>

module windowManager;

namespace windowManager {
    ViewInfo handleResize(sf::RenderWindow& window) {
        ViewInfo views;
        sf::Vector2u size = window.getSize();
        float minSize = std::min(size.x, size.y);

        float xOffset = (size.x - minSize) / 2.0f;
        float yOffset = (size.y - minSize) / 2.0f;

        views.gameView.setSize(800, 800);
        views.gameView.setCenter(400, 400);
        views.gameView.setViewport(sf::FloatRect(
            xOffset / size.x,
            yOffset / size.y,
            minSize / size.x,
            minSize / size.y
        ));

        views.boardPosition = sf::Vector2f(xOffset, yOffset);
        views.boardSize = minSize;

        views.guiView.setSize(static_cast<float>(size.x), static_cast<float>(size.y));
        views.guiView.setCenter(size.x / 2.0f, size.y / 2.0f);
        views.guiView.setViewport(sf::FloatRect(0, 0, 1, 1));

        return views;
    }

    sf::Vector2f getMouseBoardPosition(const sf::RenderWindow& window) {
        return window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
    }
}