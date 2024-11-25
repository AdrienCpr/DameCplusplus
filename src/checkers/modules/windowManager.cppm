module;

#include <SFML/Graphics.hpp>

export module windowManager;


export namespace windowManager {
    struct ViewInfo {
        sf::View gameView;
        sf::View guiView;
        sf::Vector2f boardPosition;  // Position du plateau dans la fenêtre
        float boardSize;            // Taille du plateau
    };

    ViewInfo handleResize(sf::RenderWindow& window) {
        ViewInfo views;
        sf::Vector2u size = window.getSize();
        float minSize = std::min(size.x, size.y);

        // Configuration de la vue du jeu (plateau)
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

        // Calculer la position et la taille réelle du plateau dans la fenêtre
        views.boardPosition = sf::Vector2f(xOffset, yOffset);
        views.boardSize = minSize;

        // Configuration de la vue GUI alignée avec le plateau
        views.guiView.setSize(static_cast<float>(size.x), static_cast<float>(size.y));
        views.guiView.setCenter(size.x / 2.0f, size.y / 2.0f);
        views.guiView.setViewport(sf::FloatRect(0, 0, 1, 1));

        return views;
    }

    sf::Vector2f getMouseBoardPosition(const sf::RenderWindow& window) {
        return window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
    }
}