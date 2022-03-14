#include "Piece.h"

Piece::Piece(PieceType type, Color color, Rank rank, File file) :
	type(type),
	color(color),
	rank(rank),
	file(file),
	captured(false),
	hasMoved(false)
{
}

char RankToChar(Rank rank) {
	switch (rank) {
	case Rank::R1: return '1';
	case Rank::R2: return '2';
	case Rank::R3: return '3';
	case Rank::R4: return '4';
	case Rank::R5: return '5';
	case Rank::R6: return '6';
	case Rank::R7: return '7';
	case Rank::R8: return '8';
	default: return '\0';
	}
}

char FileToChar(File file) {
	switch (file) {
	case File::A: return 'a';
	case File::B: return 'b';
	case File::C: return 'c';
	case File::D: return 'd';
	case File::E: return 'e';
	case File::F: return 'f';
	case File::G: return 'g';
	case File::H: return 'h';
	default: return '\0';
	}
}

const char* PieceTypeToString(PieceType piece) {
	switch (piece) {
	case PieceType::PAWN: return "pawn";
	case PieceType::KNIGHT: return "knight";
	case PieceType::BISHOP: return "bishop";
	case PieceType::ROOK: return "rook";
	case PieceType::QUEEN: return "queen";
	case PieceType::KING: return "king";
	default: return "(unknown)";
	}
}