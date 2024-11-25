module;

#include <SFML/Graphics.hpp>

export module game;

import board;
import position;

export namespace Game {

    void handleClick(const sf::Vector2f& worldPos, position::Position& selectedPos, board::GameBoard& gameBoard, sf::RenderWindow& window) {
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
}
