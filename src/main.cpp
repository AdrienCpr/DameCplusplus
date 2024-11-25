#include <iostream>
#include <SFML/Graphics.hpp>

import board;
import position;
import windowManager;
import game;
import piece;

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

    std::optional<PieceColor> winner;
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

        std::optional<PieceColor> winner = gameBoard.checkVictory();

        if (winner) {
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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                winner.reset();
                gameBoard = board::GameBoard(&window);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                window.close();
            }
        }
        window.display();
    }

    return 0;
}