module;

#include <SFML/Graphics.hpp>

module piece;

namespace piece {
    Piece::Piece(PieceColor color) : color(color) {}

    sf::CircleShape Piece::draw(float size) const {
        sf::CircleShape shape(size / 2);
        shape.setFillColor(color == PieceColor::White ? sf::Color( 200 , 173 , 127) : sf::Color::Black);
        return shape;
    }

    King::King(PieceColor color) : Piece(color) {}

    sf::CircleShape King::draw(float size) const {
        sf::CircleShape shape = Piece::draw(size);
        shape.setOutlineColor(sf::Color::Red);
        shape.setOutlineThickness(5);
        return shape;
    }
}
