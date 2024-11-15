module;

export module position;

export namespace position {
    struct Position {
        int row;
        int col;

        Position(int r, int c);
        bool isValid() const;

        bool operator==(const Position& other) const;
        bool operator!=(const Position& other) const;
    };
}
