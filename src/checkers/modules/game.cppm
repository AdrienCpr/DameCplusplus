module;

#include <SFML/Graphics.hpp>

export module game;

import board;
import position;

export namespace Game {

    void handleClick(sf::Event& event, position::Position& selectedPos, board::GameBoard& gameBoard) {
        float tileSize = 800.0f / board::GameBoard::size;
        int row = event.mouseButton.y / tileSize;
        int col = event.mouseButton.x / tileSize;

        position::Position clickedPos(row, col);

        if (clickedPos.isValid()) {
            if (selectedPos.row == -1) {
                if (gameBoard.getPieceAt(clickedPos)) {
                    selectedPos = clickedPos;
                }
            } else {
                if (gameBoard.isValidMove(selectedPos, clickedPos)) {
                    gameBoard.movePiece(selectedPos, clickedPos);
                }
                else if (gameBoard.isValidCapture(selectedPos, clickedPos)) {
                    gameBoard.capturePiece(selectedPos, clickedPos);
                }

                selectedPos = position::Position(-1, -1);
            }
        }
    }
}
