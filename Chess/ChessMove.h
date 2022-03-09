#pragma once

#include "Piece.h"

enum class ChessMoveType {
	NORMAL,
	CAPTURE,
	CASTLE,
};

struct CaptureData {
	Piece* capturedPiece;
	bool enPassant;
};

struct CastleData {
	Piece* rook;
	Rank rookOldRank;
	File rookOldFile;
};

struct ChessMove
{
	ChessMoveType type;
	Piece* piece;
	Rank oldRank;
	File oldFile;
	Rank newRank;
	File newFile;

	bool isPromotion;
	PieceType promoteType;

	union {
		CaptureData captureData;
		CastleData castleData;
	} extra;
};