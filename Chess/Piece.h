#pragma once

enum class PieceType {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    MAX,
};

enum class Color {
    WHITE,
    BLACK,
};

enum class Rank {
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
};

enum class File {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
};

struct Piece {
    Piece(PieceType type, Color color, Rank rank, File file);

    PieceType type;
    Color color;
    Rank rank;
    File file;
    bool captured;
    bool hasMoved;
};