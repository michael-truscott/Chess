#pragma once

#include <vector>
#include <memory>
#include <deque>
#include "Piece.h"
#include "ChessMove.h"

class BoardState
{
public:
	BoardState();

	void Reset();
	bool Rewind();

	const std::vector<std::unique_ptr<Piece>>& GetAllPieces() const;
	Piece* GetPieceAt(Rank rank, File file);
	const Color CurrentTurn() const;

	bool MovePiece(Piece* piece, Rank newRank, File newFile);
	bool IsMoveLegal(ChessMove* move);
	
	bool IsSquareUnderAttackByColor(Rank rank, File file, Color color);
	bool IsPieceAttackingSquare(Piece* piece, Rank rank, File file);
private:
	void RemovePiece(Piece* piece);
	void NextTurn();

	std::unique_ptr<ChessMove> TryCreateMove(Piece* piece, Rank newRank, File newFile);

	bool IsPathClear(Piece* piece, Rank targetRank, File targetFile);
	
	void ApplyMove(ChessMove* move);
	void UnapplyMove(ChessMove* move);

	std::vector<std::unique_ptr<Piece>> m_pieces;
	Color m_currentTurn;

	std::deque<std::unique_ptr<ChessMove>> m_moveHistory;
};
