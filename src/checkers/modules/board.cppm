module;

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

export module board;

import piece;
import position;
import soundManager;

export namespace board {
    class GameBoard {
    public:
        static constexpr int size = 8;

        GameBoard();
        void setupPieces();
        void promoteToKing(const position::Position& pos);
        void draw(sf::RenderWindow &window, const position::Position& selectedPos) const;

        bool movePiece(const position::Position& from, const position::Position& to);
        bool capturePiece(const position::Position& from, const position::Position& to);
        bool isValidMove(const position::Position& from, const position::Position& to) const;
        bool isValidCapture(const position::Position& from, const position::Position& to) const;

        std::unique_ptr<piece::Piece>& getPieceAt(const position::Position& pos);
        const std::unique_ptr<piece::Piece>& getPieceAt(const position::Position& pos) const;

    private:
        std::vector<std::vector<std::unique_ptr<piece::Piece>>> grid;
        PieceColor currentPlayer;
        std::unique_ptr<soundManager::SoundManager> soundManager;
    };
}