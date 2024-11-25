module;

#include <SFML/Graphics.hpp>

module board;

import piece;
import position;
import soundManager;

namespace board {
    GameBoard::GameBoard(sf::RenderWindow* renderWindow)
        : grid(size),
          currentPlayer(PieceColor::White),
          soundManager(std::make_unique<soundManager::SoundManager>()),
          window(renderWindow) {
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
        float duration = 0.5f;
        sf::Clock clock;

        float tileSize = 800.0f / GameBoard::size;

        sf::Vector2f startPos(from.col * tileSize, from.row * tileSize);
        sf::Vector2f endPos(to.col * tileSize, to.row * tileSize);

        auto piece = std::move(grid[from.row][from.col]);
        grid[from.row][from.col].reset();

        while (clock.getElapsedTime().asSeconds() < duration) {
            float t = clock.getElapsedTime().asSeconds() / duration;

            sf::Vector2f currentPos = startPos + t * (endPos - startPos);

            window.clear(sf::Color(50, 50, 50));
            draw(window, position::Position(-1, -1));

            if (piece) {
                auto pieceShape = piece->draw(tileSize * 0.8f);
                pieceShape.setPosition(currentPos + sf::Vector2f(tileSize * 0.1f, tileSize * 0.1f));
                window.draw(pieceShape);
            }
            window.display();
        }

        grid[to.row][to.col] = std::move(piece);
    }

    void GameBoard::animatePromotion(const position::Position& pos, sf::RenderWindow& window) {
        sf::CircleShape pieceShape;
        if (auto& piece = grid[pos.row][pos.col]) {
            float tileSize = 800.0f / size;
            float baseSize = tileSize * 0.8f / 2.0f;
            pieceShape = piece->draw(tileSize * 0.8f);
            pieceShape.setPosition(pos.col * tileSize + tileSize * 0.1f, pos.row * tileSize + tileSize * 0.1f);

            sf::Clock clock;
            float duration = 1.0f;
            while (clock.getElapsedTime().asSeconds() < duration) {
                float t = clock.getElapsedTime().asSeconds() / duration;
                float scale = 1.0f + 0.2f * std::sin(t * 3.14f * 2);
                pieceShape.setScale(scale, scale);

                pieceShape.setOutlineThickness(5 * (1.0f - t));
                pieceShape.setOutlineColor(sf::Color(255, 215, 0, static_cast<sf::Uint8>((1.0f - t) * 255)));

                window.clear(sf::Color(50, 50, 50));
                draw(window, position::Position(-1, -1));
                window.draw(pieceShape);
                window.display();
            }
        }
    }

    void board::GameBoard::promoteToKing(const position::Position& pos) {
        if (grid[pos.row][pos.col] &&
            dynamic_cast<piece::King*>(grid[pos.row][pos.col].get()) == nullptr) {
            grid[pos.row][pos.col] = std::make_unique<piece::King>(grid[pos.row][pos.col]->color);

            if (soundManager) {
                soundManager->playSound("promote");
            }

            if (window) {
                animatePromotion(pos, *window);
            }
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
                }
            }
        }

        if (selectedPos.row != -1 && selectedPos.col != -1) {
            auto validMoves = getValidMoves(selectedPos);

            PieceColor currentPlayer = getCurrentPlayer();

            auto& pieceAtSelectedPos = getPieceAt(selectedPos);
            if (pieceAtSelectedPos && pieceAtSelectedPos->color == currentPlayer) {
                for (const auto& move : validMoves) {
                    sf::CircleShape highlight(tileSize * 0.3f);
                    highlight.setFillColor(sf::Color(0, 255, 0, 150));

                    highlight.setPosition(move.col * tileSize + tileSize / 2 - highlight.getRadius(),
                                          move.row * tileSize + tileSize / 2 - highlight.getRadius());

                    window.draw(highlight);
                }
            }
        }
    }

    void GameBoard::drawGameInfo(sf::RenderWindow& window, const GameBoard& gameBoard, sf::Font& font, const windowManager::ViewInfo& views) {
        const float PADDING = 20.f;
        const float INFO_WIDTH = 220.f;
        const float INFO_HEIGHT = 100.f;
        const int CHAR_SIZE = 20;
        const float LINE_SPACING = 10.f;
        const float BORDER_PADDING = 10.f;
        const float ICON_SIZE = 10.f;
        const sf::Color WHITE_PIECE_COLOR(200, 173, 127);

        float baseX = views.boardPosition.x + views.boardSize + PADDING;
        float baseY = views.boardPosition.y + (views.boardSize / 2.f) - (INFO_HEIGHT / 2.f);

        auto [whiteCount, blackCount] = gameBoard.countPieces();

        sf::RectangleShape infoBackground(sf::Vector2f(INFO_WIDTH, INFO_HEIGHT));
        infoBackground.setPosition(baseX - BORDER_PADDING, baseY - BORDER_PADDING);
        infoBackground.setFillColor(sf::Color(50, 50, 50, 200));
        infoBackground.setOutlineColor(sf::Color::White);
        infoBackground.setOutlineThickness(2.f);

        sf::Text currentPlayerText;
        currentPlayerText.setFont(font);
        currentPlayerText.setCharacterSize(CHAR_SIZE);
        currentPlayerText.setFillColor(sf::Color::White);
        currentPlayerText.setString("Turn : ");
        currentPlayerText.setPosition(baseX + BORDER_PADDING, baseY);

        sf::CircleShape currentPlayerIcon(ICON_SIZE);
        currentPlayerIcon.setFillColor(gameBoard.getCurrentPlayer() == PieceColor::White ? WHITE_PIECE_COLOR : sf::Color::Black);
        currentPlayerIcon.setOutlineThickness(1.f);
        currentPlayerIcon.setOutlineColor(sf::Color::White);
        currentPlayerIcon.setPosition(
            currentPlayerText.getPosition().x + currentPlayerText.getLocalBounds().width + 5.f,
            currentPlayerText.getPosition().y + (CHAR_SIZE / 2.f - ICON_SIZE)
        );

        window.draw(infoBackground);
        window.draw(currentPlayerText);
        window.draw(currentPlayerIcon);

        sf::CircleShape whiteIcon(ICON_SIZE);
        whiteIcon.setFillColor(WHITE_PIECE_COLOR);
        whiteIcon.setOutlineThickness(1.f);
        whiteIcon.setOutlineColor(sf::Color::White);
        whiteIcon.setPosition(baseX + BORDER_PADDING,
            currentPlayerText.getPosition().y + currentPlayerText.getLocalBounds().height + LINE_SPACING + (CHAR_SIZE / 2.f - ICON_SIZE));

        sf::Text whiteScoreText;
        whiteScoreText.setFont(font);
        whiteScoreText.setCharacterSize(CHAR_SIZE);
        whiteScoreText.setFillColor(sf::Color::White);
        whiteScoreText.setString(": " + std::to_string(whiteCount));
        whiteScoreText.setPosition(whiteIcon.getPosition().x + ICON_SIZE * 2.f + 5.f,
            currentPlayerText.getPosition().y + currentPlayerText.getLocalBounds().height + LINE_SPACING);

        sf::CircleShape blackIcon(ICON_SIZE);
        blackIcon.setFillColor(sf::Color::Black);
        blackIcon.setOutlineThickness(1.f);
        blackIcon.setOutlineColor(sf::Color::White);
        blackIcon.setPosition(baseX + BORDER_PADDING,
            whiteScoreText.getPosition().y + whiteScoreText.getLocalBounds().height + LINE_SPACING + (CHAR_SIZE / 2.f - ICON_SIZE));

        sf::Text blackScoreText;
        blackScoreText.setFont(font);
        blackScoreText.setCharacterSize(CHAR_SIZE);
        blackScoreText.setFillColor(sf::Color::White);
        blackScoreText.setString(": " + std::to_string(blackCount));
        blackScoreText.setPosition(blackIcon.getPosition().x + ICON_SIZE * 2.f + 5.f,
            whiteScoreText.getPosition().y + whiteScoreText.getLocalBounds().height + LINE_SPACING);

        window.draw(whiteIcon);
        window.draw(whiteScoreText);
        window.draw(blackIcon);
        window.draw(blackScoreText);
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