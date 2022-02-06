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