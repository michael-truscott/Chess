#pragma once

#include <vector>
#include <memory>
#include <deque>
#include "Piece.h"

class BoardState
{
public:
	BoardState();

	void Reset();

	const std::vector<std::unique_ptr<Piece>>& GetAllPieces() const;
	Piece* GetPieceAt(Rank rank, File file);
	const Color CurrentTurn() const;

	bool MovePiece(Piece* piece, Rank newRank, File newFile);
private:
	void RemovePiece(Piece* piece);
	void NextTurn();

	std::vector<std::unique_ptr<Piece>> m_pieces;
	Color m_currentTurn;
};
