module;

#include <SFML/Graphics.hpp>
#include <optional>

export module game;

import board;
import position;
import windowManager;
import piece;

export namespace Game {
    class GameState {
    public:
        GameState(sf::RenderWindow* window);
        void handleEvents(sf::RenderWindow& window, windowManager::ViewInfo& views);
        void update(sf::RenderWindow& window);
        void render(sf::RenderWindow& window, windowManager::ViewInfo& views);
        bool isGameRunning() const;

    private:
        board::GameBoard gameBoard;
        position::Position selectedPos;
        std::optional<PieceColor> winner;
        bool isPaused;
        sf::Font font;
        sf::RectangleShape quitButton;
        sf::RenderWindow* gameWindow;

        void handleGameClick(const sf::Vector2f& worldPos, sf::RenderWindow& window);
        void handlePauseClick(const sf::Vector2f& mousePos, sf::RenderWindow& window);
        void drawPauseScreen(sf::RenderWindow& window);
        void drawWinScreen(sf::RenderWindow& window);
    };
}