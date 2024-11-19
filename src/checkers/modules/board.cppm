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

        GameBoard(sf::RenderWindow* renderWindow);
        void setupPieces();
        void promoteToKing(const position::Position& pos);
        void draw(sf::RenderWindow &window, const position::Position& selectedPos) const;
        void animatePromotion(const position::Position& pos, sf::RenderWindow& window);

        static void drawGameInfo(sf::RenderWindow& window, const GameBoard& gameBoard, sf::Font& font);
        bool movePiece(const position::Position& from, const position::Position& to, sf::RenderWindow& window);
        bool capturePiece(const position::Position& from, const position::Position& to);
        bool isValidMove(const position::Position& from, const position::Position& to) const;
        bool isValidCapture(const position::Position& from, const position::Position& to) const;
        std::unique_ptr<piece::Piece>& getPieceAt(const position::Position& pos);
        const std::unique_ptr<piece::Piece>& getPieceAt(const position::Position& pos) const;
        PieceColor getCurrentPlayer() const;
        void animatePieceMove(const position::Position& from, const position::Position& to, sf::RenderWindow& window); // Animation des déplacements

        std::pair<int, int> countPieces() const {
            int whiteCount = 0, blackCount = 0;
            for (const auto& row : grid) {
                for (const auto& piece : row) {
                    if (piece) {
                        if (piece->color == PieceColor::White) {
                            ++whiteCount;
                        } else if (piece->color == PieceColor::Black) {
                            ++blackCount;
                        }
                    }
                }
            }
            return {whiteCount, blackCount};
        }

        std::vector<position::Position> getValidMoves(const position::Position& from) const {
            std::vector<position::Position> validMoves;

            if (getPieceAt(from)) {
                for (int row = 0; row < size; ++row) {
                    for (int col = 0; col < size; ++col) {
                        position::Position to(row, col);
                        if (isValidMove(from, to) || isValidCapture(from, to)) {
                            validMoves.push_back(to);
                        }
                    }
                }
            }

            return validMoves;
        }

    private:
        std::vector<std::vector<std::unique_ptr<piece::Piece>>> grid;
        PieceColor currentPlayer;
        std::unique_ptr<soundManager::SoundManager> soundManager;
        sf::RenderWindow* window = nullptr;
    };
}
