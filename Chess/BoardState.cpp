#include "BoardState.h"

BoardState::BoardState() :
	m_currentTurn(Color::WHITE)
{
	Reset();
}

void BoardState::Reset()
{
	m_pieces.clear();

	m_pieces.push_back(std::make_unique<Piece>(PieceType::ROOK, Color::WHITE, Rank::R1, File::A));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KNIGHT, Color::WHITE, Rank::R1, File::B));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::BISHOP, Color::WHITE, Rank::R1, File::C));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::QUEEN, Color::WHITE, Rank::R1, File::D));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KING, Color::WHITE, Rank::R1, File::E));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::BISHOP, Color::WHITE, Rank::R1, File::F));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KNIGHT, Color::WHITE, Rank::R1, File::G));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::ROOK, Color::WHITE, Rank::R1, File::H));

	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::A));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::B));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::C));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::D));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::E));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::F));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::G));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::WHITE, Rank::R2, File::H));

	m_pieces.push_back(std::make_unique<Piece>(PieceType::ROOK, Color::BLACK, Rank::R8, File::A));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KNIGHT, Color::BLACK, Rank::R8, File::B));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::BISHOP, Color::BLACK, Rank::R8, File::C));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::QUEEN, Color::BLACK, Rank::R8, File::D));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KING, Color::BLACK, Rank::R8, File::E));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::BISHOP, Color::BLACK, Rank::R8, File::F));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::KNIGHT, Color::BLACK, Rank::R8, File::G));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::ROOK, Color::BLACK, Rank::R8, File::H));

	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::A));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::B));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::C));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::D));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::E));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::F));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::G));
	m_pieces.push_back(std::make_unique<Piece>(PieceType::PAWN, Color::BLACK, Rank::R7, File::H));
	
	m_currentTurn = Color::WHITE;
}

const std::vector<std::unique_ptr<Piece>>& BoardState::GetAllPieces() const
{
	return m_pieces;
}

Piece* BoardState::GetPieceAt(Rank rank, File file)
{
	for (auto& piece : m_pieces) {
		if (piece->rank == rank && piece->file == file) {
			return piece.get();
		}
	}
	return nullptr;
}

const Color BoardState::CurrentTurn() const
{
	return m_currentTurn;
}

/// <summary>
/// Attempt to move a piece to a new position, performing any captures, promotions, etc. as necessary.
/// Returns true if the move was legal & successful.
/// </summary>
/// <param name="piece">The piece to move.</param>
/// <param name="newRank"></param>
/// <param name="newFile"></param>
/// <returns>True if the move succeeded, false otherwise.</returns>
bool BoardState::MovePiece(Piece* piece, Rank newRank, File newFile)
{
	// TODO: do move validation, store move history, etc.
	Piece* occupyingPiece = GetPieceAt(newRank, newFile);
	if (occupyingPiece) {
		if (occupyingPiece->color == piece->color)
			return false; // can't capture a piece of your own colour
		else
			RemovePiece(occupyingPiece);
	}

	piece->rank = newRank;
	piece->file = newFile;
	NextTurn();
	return true;
}

void BoardState::RemovePiece(Piece* piece)
{
	m_pieces.erase(
		std::remove_if(m_pieces.begin(), m_pieces.end(),
			[piece](const std::unique_ptr<Piece>& p) { return p.get() == piece; })
	);
}

void BoardState::NextTurn()
{
	m_currentTurn = m_currentTurn == Color::WHITE ? Color::BLACK : Color::WHITE;
}
