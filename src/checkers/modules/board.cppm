module;

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

export module board;

import piece;
import position;

export namespace board {
    class GameBoard {
    public:
        static constexpr int size = 8;

        GameBoard();
        void setupPieces();
        void promoteToKing(const position::Position& pos);
        void draw(sf::RenderWindow &window) const;

    private:
        std::vector<std::vector<std::unique_ptr<piece::Piece>>> grid;
    };
}