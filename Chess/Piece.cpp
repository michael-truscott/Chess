#include "Piece.h"

Piece::Piece(PieceType type, Color color, Rank rank, File file)
{
	this->type = type;
	this->color = color;
	this->rank = rank;
	this->file = file;
}