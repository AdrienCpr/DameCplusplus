module;

#include <SFML/Graphics.hpp>

export module piece;

export enum class PieceColor { None, White, Black };

export namespace piece {
    class Piece {
    public:
        PieceColor color;
        explicit Piece(PieceColor color = PieceColor::None);
        virtual ~Piece() = default;
        virtual sf::CircleShape draw(float size) const;
    };

    class King : public Piece {
    public:
        explicit King(PieceColor color);
        sf::CircleShape draw(float size) const override;
    };
}