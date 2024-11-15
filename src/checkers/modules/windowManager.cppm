module;

#include <SFML/Graphics.hpp>

export module windowManager;

export namespace windowManager {

    void handleResize(sf::RenderWindow& window, sf::View& fixedView) {
        sf::Vector2u size = window.getSize();
        float minSize = std::min(size.x, size.y);

        float xOffset = (size.x - minSize) / 2.0f;
        float yOffset = (size.y - minSize) / 2.0f;

        fixedView.setViewport(sf::FloatRect(
            xOffset / size.x,
            yOffset / size.y,
            minSize / size.x,
            minSize / size.y
        ));

        window.setView(fixedView);
    }
}
