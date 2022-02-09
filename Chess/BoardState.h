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
	const Piece* GetPieceAt(Rank rank, File file) const;
	const Color CurrentTurn() const;

	bool MovePiece(Piece* piece, Rank newRank, File newFile);
	bool IsMoveLegal(ChessMove* move) const;
	
	bool IsSquareUnderAttackByColor(Rank rank, File file, Color color) const;
	bool IsPieceAttackingSquare(Piece* piece, Rank rank, File file) const;

	bool IsPositionInCheck(Color color) const;

	const ChessMove* LastMove() const;
private:
	bool IsMovePositionLegal(ChessMove* move) const;

	void RemovePiece(Piece* piece);
	void NextTurn();

	std::unique_ptr<ChessMove> TryCreateMove(Piece* piece, Rank newRank, File newFile) const;

	bool IsPathClear(Piece* piece, Rank targetRank, File targetFile) const;
	
	void ApplyMove(ChessMove* move);
	void UnapplyMove(ChessMove* move);

	// eugh
	void ApplyMove(ChessMove* move) const;
	void UnapplyMove(ChessMove* move) const;

	// Declared mutable since IsMoveLegal is conceptually const but needs to temporarily modify the board state in order to
	// determine whether a move will put the player into check.
	mutable std::vector<std::unique_ptr<Piece>> m_pieces;
	Color m_currentTurn;

	std::deque<std::unique_ptr<ChessMove>> m_moveHistory;
};
