#pragma once

#include "Piece.h"

enum class ChessMoveType {
	NORMAL,
	CAPTURE,
};

struct CaptureData {
	Piece* capturedPiece;
};

struct ChessMove
{
	ChessMoveType type;
	Piece* piece;
	Rank oldRank;
	File oldFile;
	Rank newRank;
	File newFile;

	union {
		CaptureData captureData;
	} extra;
};