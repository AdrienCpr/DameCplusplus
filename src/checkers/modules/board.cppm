module;

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <numeric>

export module board;

import piece;
import position;
import soundManager;
import windowManager;

export namespace board {
    class GameBoard {
    public:
        static constexpr int size = 8;

        explicit GameBoard(sf::RenderWindow* renderWindow) noexcept;
        void setupPieces() noexcept;
        void promoteToKing(const position::Position& pos);
        void draw(sf::RenderWindow& window, const position::Position& selectedPos) const;
        void animatePromotion(const position::Position& pos, sf::RenderWindow& window);

        static void drawGameInfo(sf::RenderWindow& window, const GameBoard& gameBoard, sf::Font& font, const windowManager::ViewInfo& views);

        [[nodiscard]] bool movePiece(const position::Position& from, const position::Position& to, sf::RenderWindow& window);
        [[nodiscard]] bool capturePiece(const position::Position& from, const position::Position& to);
        [[nodiscard]] bool isValidMove(const position::Position& from, const position::Position& to) const;
        [[nodiscard]] bool isValidCapture(const position::Position& from, const position::Position& to) const;

        std::unique_ptr<piece::Piece>& getPieceAt(const position::Position& pos);
        const std::unique_ptr<piece::Piece>& getPieceAt(const position::Position& pos) const;

        [[nodiscard]] PieceColor getCurrentPlayer() const;
        void animatePieceMove(const position::Position& from, const position::Position& to, sf::RenderWindow& window);

        [[nodiscard]] std::pair<int, int> countPieces() const {
            std::pair<int, int> result{0, 0};
            for (const auto& row : grid) {
                for (const auto& piece : row) {
                    if (!piece) continue;
                    if (piece->color == PieceColor::White) result.first++;
                    else if (piece->color == PieceColor::Black) result.second++;
                }
            }
            return result;
        }

        [[nodiscard]] std::optional<PieceColor> checkVictory() const {
            auto [whiteCount, blackCount] = countPieces();

            if (whiteCount == 0) return PieceColor::Black;
            if (blackCount == 0) return PieceColor::White;

            bool whiteHasMoves = false, blackHasMoves = false;
            for (int row = 0; row < size; ++row) {
                for (int col = 0; col < size; ++col) {
                    position::Position pos(row, col);
                    const auto& piece = getPieceAt(pos);
                    if (piece) {
                        auto moves = getValidMoves(pos);
                        if (!moves.empty()) {
                            if (piece->color == PieceColor::White) whiteHasMoves = true;
                            if (piece->color == PieceColor::Black) blackHasMoves = true;
                        }
                    }
                }
            }

            if (!whiteHasMoves) return PieceColor::Black;
            if (!blackHasMoves) return PieceColor::White;

            return std::nullopt;
        }

        [[nodiscard]] std::vector<position::Position> getValidMoves(const position::Position& from) const {
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