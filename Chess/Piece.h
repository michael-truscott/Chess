#pragma once

#include <cstdint>

enum class PieceType : uint8_t {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    MAX,
};

enum class Color : uint8_t {
    WHITE,
    BLACK,
};

enum class Rank : uint8_t {
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
};

enum class File : uint8_t {
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