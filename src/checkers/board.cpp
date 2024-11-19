module;

#include <SFML/Graphics.hpp>

module board;

import piece;
import position;
import soundManager;

namespace board {
    GameBoard::GameBoard()
        : grid(size),
          currentPlayer(PieceColor::White),
          soundManager(std::make_unique<soundManager::SoundManager>()) {
        for (auto& row : grid) {
            row.resize(size);
        }
        setupPieces();
    }

    void GameBoard::setupPieces() {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < size; ++col) {
                if ((row + col) % 2 != 0) {
                    grid[row][col] = std::make_unique<piece::Piece>(PieceColor::Black);
                }
            }
        }
        for (int row = size - 3; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                if ((row + col) % 2 != 0) {
                    grid[row][col] = std::make_unique<piece::Piece>(PieceColor::White);
                }
            }
        }
    }

    void GameBoard::animatePieceMove(const position::Position& from, const position::Position& to, sf::RenderWindow& window) {
        float duration = 0.5f; // Durée de l'animation en secondes
        sf::Clock clock;

        // Taille de la case
        float tileSize = 800.0f / GameBoard::size;

        // Positions de départ et d'arrivée en pixels
        sf::Vector2f startPos(from.col * tileSize, from.row * tileSize);
        sf::Vector2f endPos(to.col * tileSize, to.row * tileSize);

        // Récupérer la pièce à animer
        auto piece = std::move(grid[from.row][from.col]);
        grid[from.row][from.col].reset();

        // Boucle pour interpoler
        while (clock.getElapsedTime().asSeconds() < duration) {
            float t = clock.getElapsedTime().asSeconds() / duration;

            // Calcul de la position interpolée
            sf::Vector2f currentPos = startPos + t * (endPos - startPos);

            // Effacer et redessiner tout
            window.clear(sf::Color(50, 50, 50)); // Couleur de fond
            draw(window, position::Position(-1, -1)); // Dessiner le plateau
            // Dessiner la pièce en cours de mouvement
            if (piece) {
                auto pieceShape = piece->draw(tileSize * 0.8f);
                pieceShape.setPosition(currentPos + sf::Vector2f(tileSize * 0.1f, tileSize * 0.1f));
                window.draw(pieceShape);
            }
            window.display();
        }

        // Place la pièce dans sa position finale
        grid[to.row][to.col] = std::move(piece);
    }

    void GameBoard::promoteToKing(const position::Position& pos) {
        if (grid[pos.row][pos.col] &&
            dynamic_cast<piece::King*>(grid[pos.row][pos.col].get()) == nullptr) {
            grid[pos.row][pos.col] = std::make_unique<piece::King>(grid[pos.row][pos.col]->color);
        }
    }

    void GameBoard::draw(sf::RenderWindow &window, const position::Position& selectedPos) const {
        const float tileSize = 800.0f / size;

        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                sf::RectangleShape tile({tileSize, tileSize});
                tile.setPosition(col * tileSize, row * tileSize);
                tile.setFillColor((row + col) % 2 == 0 ? sf::Color(254, 254, 226) : sf::Color(139, 69, 19));
                window.draw(tile);

                if (grid[row][col]) {
                    auto pieceShape = grid[row][col]->draw(tileSize * 0.8f);
                    pieceShape.setPosition(col * tileSize + tileSize * 0.1f, row * tileSize + tileSize * 0.1f);
                    window.draw(pieceShape);

                    if (selectedPos.row == row && selectedPos.col == col && grid[row][col]->color == currentPlayer) {
                        sf::RectangleShape highlight({tileSize * 0.9f, tileSize * 0.9f});
                        highlight.setFillColor(sf::Color::Transparent);
                        highlight.setOutlineColor(sf::Color::Yellow);
                        highlight.setOutlineThickness(3);
                        highlight.setPosition(
                            col * tileSize + tileSize * 0.05f,
                            row * tileSize + tileSize * 0.05f
                        );
                        window.draw(highlight);
                    }
                }
            }
        }
    }

    void GameBoard::drawGameInfo(sf::RenderWindow& window, const GameBoard& gameBoard, sf::Font& font) {
        auto [whiteCount, blackCount] = gameBoard.countPieces();

        sf::Text currentPlayerText;
        currentPlayerText.setFont(font);
        currentPlayerText.setCharacterSize(20);
        currentPlayerText.setFillColor(sf::Color::Green);
        currentPlayerText.setString("Tour actuel : " +
            std::string(gameBoard.getCurrentPlayer() == PieceColor::White ? "Blanc" : "Noir"));
        currentPlayerText.setPosition(10.f, 10.f);
        window.draw(currentPlayerText);

        sf::Text pieceCountText;
        pieceCountText.setFont(font);
        pieceCountText.setCharacterSize(20);
        pieceCountText.setFillColor(sf::Color::Green);
        pieceCountText.setString("Blancs : " + std::to_string(whiteCount) +
            " | Noirs : " + std::to_string(blackCount));
        pieceCountText.setPosition(10.f, 40.f);
        window.draw(pieceCountText);
    }

    PieceColor GameBoard::getCurrentPlayer() const {
        return currentPlayer;
    }

    bool GameBoard::isValidMove(const position::Position& from, const position::Position& to) const {
        if (!to.isValid() || grid[to.row][to.col]) {
            return false;
        }

        int rowDiff = to.row - from.row;
        int colDiff = std::abs(to.col - from.col);

        auto piece = grid[from.row][from.col].get();

        if (dynamic_cast<piece::King*>(piece)) {
            return (std::abs(rowDiff) == std::abs(colDiff) && std::abs(rowDiff) == 1);
        } else {
            int allowedDirection = (piece->color == PieceColor::White) ? -1 : 1;
            return (rowDiff == allowedDirection && colDiff == 1);
        }
    }


    bool GameBoard::isValidCapture(const position::Position& from, const position::Position& to) const {
        if (!to.isValid() || grid[to.row][to.col]) {
            return false;
        }

        int rowDiff = to.row - from.row;
        int colDiff = to.col - from.col;

        auto piece = grid[from.row][from.col].get();

        if (dynamic_cast<piece::King*>(piece)) {
            if (std::abs(rowDiff) > 1 && std::abs(rowDiff) == std::abs(colDiff)) {
                int midRow = (from.row + to.row) / 2;
                int midCol = (from.col + to.col) / 2;

                return grid[midRow][midCol] && grid[midRow][midCol]->color != piece->color;
            }
        } else {
            int allowedDirection = (piece->color == PieceColor::White) ? -2 : 2;
            if (rowDiff == allowedDirection && std::abs(colDiff) == 2) {
                int midRow = (from.row + to.row) / 2;
                int midCol = (from.col + to.col) / 2;

                return grid[midRow][midCol] && grid[midRow][midCol]->color != piece->color;
            }
        }

        return false;
    }


    bool GameBoard::movePiece(const position::Position& from, const position::Position& to, sf::RenderWindow& window) {
        if (grid[from.row][from.col] && grid[from.row][from.col]->color == currentPlayer) {
            if (isValidMove(from, to)) {
                animatePieceMove(from, to, window);

                if (to.row == 0 || to.row == size - 1) {
                    promoteToKing(to);
                    soundManager->playSound("promote");
                } else {
                    soundManager->playSound("move");
                }

                currentPlayer = (currentPlayer == PieceColor::White) ? PieceColor::Black : PieceColor::White;
                return true;
            }
        }
        return false;
    }

    bool GameBoard::capturePiece(const position::Position& from, const position::Position& to) {
        if (isValidCapture(from, to)) {
            int midRow = (from.row + to.row) / 2;
            int midCol = (from.col + to.col) / 2;

            grid[to.row][to.col] = std::move(grid[from.row][from.col]);
            grid[from.row][from.col].reset();
            grid[midRow][midCol].reset();

            if (to.row == 0 || to.row == size - 1) {
                promoteToKing(to);
                soundManager->playSound("promote");
            } else {
                soundManager->playSound("capture");
            }

            return true;
        }
        return false;
    }


    std::unique_ptr<piece::Piece>& GameBoard::getPieceAt(const position::Position& pos) {
        return grid[pos.row][pos.col];
    }

    const std::unique_ptr<piece::Piece>& GameBoard::getPieceAt(const position::Position& pos) const {
        return grid[pos.row][pos.col];
    }
}