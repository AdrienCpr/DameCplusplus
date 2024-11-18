module;

#include <SFML/Graphics.hpp>

module board;

import piece;
import position;

namespace board {
    GameBoard::GameBoard() : grid(size), currentPlayer(PieceColor::White) {
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

    void GameBoard::promoteToKing(const position::Position& pos) {
        if (grid[pos.row][pos.col] &&
            dynamic_cast<piece::King*>(grid[pos.row][pos.col].get()) == nullptr) {
            grid[pos.row][pos.col] = std::make_unique<piece::King>(grid[pos.row][pos.col]->color);
        }
    }

    void GameBoard::draw(sf::RenderWindow &window) const {
        const float tileSize = 800.0f / size;

        for (int row = 0; row < size; ++row) {
            for (int col = 0; col < size; ++col) {
                sf::RectangleShape tile({tileSize, tileSize});
                tile.setPosition(col * tileSize, row * tileSize);
                tile.setFillColor((row + col) % 2 == 0 ? sf::Color( 254 , 254 , 226) : sf::Color(139, 69, 19));
                window.draw(tile);

                if (grid[row][col]) {
                    auto pieceShape = grid[row][col]->draw(tileSize * 0.8f);
                    pieceShape.setPosition(col * tileSize + tileSize * 0.1f, row * tileSize + tileSize * 0.1f);
                    window.draw(pieceShape);
                }
            }
        }
    }

    bool GameBoard::isValidMove(const position::Position& from, const position::Position& to) const {
        if (!to.isValid() || grid[to.row][to.col]) {
            return false;
        }

        int rowDiff = std::abs(to.row - from.row);
        int colDiff = std::abs(to.col - from.col);

        auto piece = grid[from.row][from.col].get();

        if (dynamic_cast<piece::King*>(piece)) {
            return (rowDiff == colDiff && rowDiff > 0);
        } else {
            return (rowDiff == 1 && colDiff == 1);
        }
    }


    bool GameBoard::isValidCapture(const position::Position& from, const position::Position& to) const {
        if (!to.isValid() || grid[to.row][to.col]) {
            return false;
        }

        int rowDiff = std::abs(to.row - from.row);
        int colDiff = std::abs(to.col - from.col);

        auto piece = grid[from.row][from.col].get();

        if (dynamic_cast<piece::King*>(piece)) {
            if (rowDiff > 2 && colDiff > 2) {
                int midRow = (from.row + to.row) / 2;
                int midCol = (from.col + to.col) / 2;

                if (grid[midRow][midCol] && grid[midRow][midCol]->color != piece->color) {
                    return true;
                }
            }
        } else {
            if (rowDiff == 2 && colDiff == 2) {
                int midRow = (from.row + to.row) / 2;
                int midCol = (from.col + to.col) / 2;
                if (grid[midRow][midCol] && grid[midRow][midCol]->color != piece->color) {
                    return true;
                }
            }
        }

        return false;
    }


    bool GameBoard::movePiece(const position::Position& from, const position::Position& to) {
        if (grid[from.row][from.col] && grid[from.row][from.col]->color == currentPlayer) {
            if (isValidMove(from, to)) {
                grid[to.row][to.col] = std::move(grid[from.row][from.col]);
                grid[from.row][from.col].reset();

                if (to.row == 0 || to.row == size - 1) {
                    promoteToKing(to);
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