module;

#include <iostream>
#include <SFML/Graphics.hpp>

module game;

namespace Game {
    GameState::GameState(sf::RenderWindow* window)
        : gameBoard(window), selectedPos(-1, -1), winner(std::nullopt),
          isPaused(false), gameWindow(window) {
        if (!font.loadFromFile("resources/fonts/arial.ttf")) {
            std::cerr << "Erreur : Impossible de charger la police." << std::endl;
            throw std::runtime_error("Font loading failed");
        }

        quitButton.setSize(sf::Vector2f(200, 50));
        quitButton.setFillColor(sf::Color(150, 0, 0));
    }

    void GameState::handleEvents(sf::RenderWindow& window, windowManager::ViewInfo& views) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::Resized) {
                views = windowManager::handleResize(window);
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    isPaused = !isPaused;
                }

                if (winner && event.key.code == sf::Keyboard::R) {
                    winner.reset();
                    gameBoard = board::GameBoard(gameWindow);
                } else if (winner && event.key.code == sf::Keyboard::Q) {
                    window.close();
                }
            }

            if (!isPaused && event.type == sf::Event::MouseButtonPressed) {
                window.setView(views.gameView);
                sf::Vector2f worldPos = windowManager::getMouseBoardPosition(window);
                handleGameClick(worldPos, window);
            }

            if (isPaused && event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), views.guiView);
                    handlePauseClick(mousePos, window);
                }
            }
        }
    }

    void GameState::handleGameClick(const sf::Vector2f& worldPos, sf::RenderWindow& window) {
        float tileSize = 800.0f / board::GameBoard::size;
        int row = static_cast<int>(worldPos.y / tileSize);
        int col = static_cast<int>(worldPos.x / tileSize);

        position::Position clickedPos(row, col);

        if (clickedPos.isValid()) {
            if (selectedPos.row == -1) {
                if (gameBoard.getPieceAt(clickedPos)) {
                    selectedPos = clickedPos;
                }
            } else {
                if (gameBoard.isValidMove(selectedPos, clickedPos)) {
                    gameBoard.movePiece(selectedPos, clickedPos, window);
                }
                else if (gameBoard.isValidCapture(selectedPos, clickedPos)) {
                    gameBoard.capturePiece(selectedPos, clickedPos);
                }

                selectedPos = position::Position(-1, -1);
            }
        }
    }

    void GameState::handlePauseClick(const sf::Vector2f& mousePos, sf::RenderWindow& window) {
        if (quitButton.getGlobalBounds().contains(mousePos)) {
            window.close();
        }
    }

    void GameState::update(sf::RenderWindow& window) {
        if (!isPaused && !winner) {
            winner = gameBoard.checkVictory();
        }
    }

    void GameState::render(sf::RenderWindow& window, windowManager::ViewInfo& views) {
        window.clear(sf::Color(50, 50, 50));

        if (isPaused) {
            drawPauseScreen(window);
        } else {
            window.setView(views.gameView);
            gameBoard.draw(window, selectedPos);

            window.setView(views.guiView);
            gameBoard.drawGameInfo(window, gameBoard, font, views);

            if (winner) {
                drawWinScreen(window);
            }
        }

        window.display();
    }

    void GameState::drawPauseScreen(sf::RenderWindow& window) {
        sf::RectangleShape blurOverlay(sf::Vector2f(window.getSize()));
        blurOverlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(blurOverlay);

        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setCharacterSize(50);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setString("Pause");
        pauseText.setPosition(window.getSize().x / 2.f - pauseText.getGlobalBounds().width / 2.f,
                              window.getSize().y / 2.f - pauseText.getGlobalBounds().height / 2.f - 50.f);
        window.draw(pauseText);

        quitButton.setPosition(window.getSize().x / 2.f - quitButton.getSize().x / 2.f,
                               pauseText.getPosition().y + pauseText.getGlobalBounds().height + 30.f);
        window.draw(quitButton);

        sf::Text quitText;
        quitText.setFont(font);
        quitText.setCharacterSize(20);
        quitText.setFillColor(sf::Color::White);
        quitText.setString("Quit the game");
        quitText.setPosition(quitButton.getPosition().x + quitButton.getSize().x / 2.f - quitText.getGlobalBounds().width / 2.f,
                             quitButton.getPosition().y + quitButton.getSize().y / 2.f - quitText.getGlobalBounds().height / 2.f);
        window.draw(quitText);

        sf::Text instructionsText;
        instructionsText.setFont(font);
        instructionsText.setCharacterSize(20);
        instructionsText.setFillColor(sf::Color::White);
        instructionsText.setString("Press ESC to Resume");
        instructionsText.setPosition(window.getSize().x / 2.f - instructionsText.getGlobalBounds().width / 2.f,
                                      quitButton.getPosition().y + quitButton.getSize().y + 20.f);
        window.draw(instructionsText);
    }

    void GameState::drawWinScreen(sf::RenderWindow& window) {
        sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        sf::Text winText;
        winText.setFont(font);
        winText.setCharacterSize(50);
        winText.setFillColor(sf::Color::White);
        winText.setString((*winner == PieceColor::White ? "White" : "Black") + std::string(" Wins!"));
        winText.setPosition(window.getSize().x / 2.f - winText.getGlobalBounds().width / 2.f,
                            window.getSize().y / 2.f - winText.getGlobalBounds().height / 2.f);
        window.draw(winText);

        sf::Text restartText;
        restartText.setFont(font);
        restartText.setCharacterSize(30);
        restartText.setFillColor(sf::Color::White);
        restartText.setString("Press R to Restart or Q to Quit");
        restartText.setPosition(window.getSize().x / 2.f - restartText.getGlobalBounds().width / 2.f,
                                winText.getPosition().y + winText.getGlobalBounds().height + 20.f);
        window.draw(restartText);
    }

    bool GameState::isGameRunning() const {
        return !winner.has_value();
    }
}