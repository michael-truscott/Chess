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

bool BoardState::Rewind()
{
	if (m_moveHistory.empty())
		return false;

	auto& move = m_moveHistory.back();
	UnapplyMove(move.get());
	NextTurn();
	m_moveHistory.pop_back();
	return true;
}

const std::vector<std::unique_ptr<Piece>>& BoardState::GetAllPieces() const
{
	return m_pieces;
}

Piece* BoardState::GetPieceAt(Rank rank, File file)
{
	for (auto& piece : m_pieces) {
		if (!piece->captured && piece->rank == rank && piece->file == file) {
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
	auto move = TryCreateMove(piece, newRank, newFile);
	if (!move || !IsMoveLegal(move.get()))
		return false;

	ApplyMove(move.get());
	m_moveHistory.push_back(std::move(move));
	NextTurn();
	return true;
}

bool BoardState::IsMoveLegal(ChessMove* move)
{
	// TODO:
	// - Check/checkmate detection
	// - Pawns: en passant
	// - Castling
	int rankDelta = (int)move->newRank - (int)move->oldRank;
	int fileDelta = (int)move->newFile - (int)move->oldFile;
	int absRankDelta = std::abs(rankDelta);
	int absFileDelta = std::abs(fileDelta);
	
	switch (move->piece->type) {
	case PieceType::PAWN:
	{
		if (!IsPathClear(move->piece, move->newRank, move->newFile))
			break;
		int oneSpace = move->piece->color == Color::WHITE ? 1 : -1;
		int twoSpaces = move->piece->color == Color::WHITE ? 2 : -2;
		if ((fileDelta == 0 && rankDelta == oneSpace) ||
			(!move->piece->hasMoved && rankDelta == twoSpaces)) {
			return true;
		}

		if (move->type == ChessMoveType::CAPTURE) {
			if (rankDelta == oneSpace && absFileDelta == 1)
				return true;
		}
		break;
	}
	case PieceType::KNIGHT:
	{
		if (absRankDelta == 1 && absFileDelta == 2 ||
			absRankDelta == 2 && absFileDelta == 1)
			return true;
		break;
	}
	case PieceType::BISHOP:
	{
		if (!IsPathClear(move->piece, move->newRank, move->newFile))
			break;
		if (absRankDelta == absFileDelta)
			return true;
		break;
	}
	case PieceType::ROOK:
	{
		if (!IsPathClear(move->piece, move->newRank, move->newFile))
			break;
		if (absRankDelta == 0 && absFileDelta != 0 ||
			absRankDelta != 0 && absFileDelta == 0)
			return true;
		break;
	}
	case PieceType::QUEEN:
		if (!IsPathClear(move->piece, move->newRank, move->newFile))
			break;
		if (absRankDelta == 0 && absFileDelta != 0 ||
			absRankDelta != 0 && absFileDelta == 0 ||
			absRankDelta == absFileDelta)
			return true;
		break;
	case PieceType::KING:
		if (!IsPathClear(move->piece, move->newRank, move->newFile))
			break;
		if (absFileDelta <= 1 && absRankDelta <= 1)
			return true;
		break;
	}
	return false;
}

void BoardState::RemovePiece(Piece* piece)
{
	piece->captured = true;
}

void BoardState::NextTurn()
{
	m_currentTurn = m_currentTurn == Color::WHITE ? Color::BLACK : Color::WHITE;
}

/// <summary>
/// Try and create a ChessMove instance representing the given move for the piece.
/// Returns a unique_ptr holding the move if successful, or holding null if not.
/// </summary>
/// <param name="piece">The piece to move.</param>
/// <param name="newRank"></param>
/// <param name="newFile"></param>
/// <returns></returns>
std::unique_ptr<ChessMove> BoardState::TryCreateMove(Piece* piece, Rank newRank, File newFile)
{
	std::unique_ptr<ChessMove> nullResult = std::unique_ptr<ChessMove>();

	Piece* occupyingPiece = GetPieceAt(newRank, newFile);
	if (occupyingPiece) {
		if (occupyingPiece->color == piece->color)
			return std::unique_ptr<ChessMove>(); // can't capture a piece of your own colour
		else {
			auto move = std::make_unique<ChessMove>();
			move->type = ChessMoveType::CAPTURE;
			move->piece = piece;
			move->oldRank = piece->rank;
			move->oldFile = piece->file;
			move->newRank = newRank;
			move->newFile = newFile;
			move->extra.captureData.capturedPiece = occupyingPiece;
			return move;
		}
	}

	auto move = std::make_unique<ChessMove>();
	move->type = ChessMoveType::NORMAL;
	move->piece = piece;
	move->oldRank = piece->rank;
	move->oldFile = piece->file;
	move->newRank = newRank;
	move->newFile = newFile;
	return move;
}

bool BoardState::IsPathClear(Piece* piece, Rank targetRank, File targetFile)
{
	// This will only work on horizontal/vertical/diagonal paths, should never be used for irregular paths e.g. knight L-shaped moves.
	int rankDelta = (int)targetRank - (int)piece->rank;
	int fileDelta = (int)targetFile - (int)piece->file;

	int rankStep;
	if (rankDelta < 0)
		rankStep = -1;
	else if (rankDelta == 0)
		rankStep = 0;
	else
		rankStep = 1;

	int fileStep;
	if (fileDelta < 0)
		fileStep = -1;
	else if (fileDelta == 0)
		fileStep = 0;
	else
		fileStep = 1;

	int rank = (int)piece->rank + rankStep;
	int file = (int)piece->file + fileStep;
	while (true) {
		if (rank == (int)targetRank && file == (int)targetFile)
			return true;

		if (GetPieceAt((Rank)rank, (File)file) != nullptr)
			return false;

		rank += rankStep;
		file += fileStep;
	}
}

bool BoardState::IsSquareUnderAttackByColor(Rank rank, File file, Color color)
{
	for (auto& piece : m_pieces) {
		if (piece->captured || piece->color != color)
			continue;

		if (IsPieceAttackingSquare(piece.get(), rank, file))
			return true;
	}
	return false;
}

bool BoardState::IsPieceAttackingSquare(Piece* piece, Rank rank, File file)
{
	if (piece->rank == rank && piece->file == file)
		return false; // can't attack ourselves!

	int rankDelta = (int)rank - (int)piece->rank;
	int fileDelta = (int)file - (int)piece->file;
	int absRankDelta = std::abs(rankDelta);
	int absFileDelta = std::abs(fileDelta);

	switch (piece->type) {
	case PieceType::PAWN:
	{
		if (piece->color == Color::WHITE && rankDelta == 1 && absFileDelta == 1)
			return true;
		if (piece->color == Color::BLACK && rankDelta == -1 && absFileDelta == 1)
			return true;

		return false;
	}
	case PieceType::KNIGHT:
	{
		return (absRankDelta == 1 && absFileDelta == 2 ||
			absRankDelta == 2 && absFileDelta == 1);
	}
	case PieceType::BISHOP:
	{
		return absRankDelta == absFileDelta && IsPathClear(piece, rank, file);
	}
	case PieceType::ROOK:
	{
		bool validMoveDirection = absRankDelta > 0 && absFileDelta == 0 || absRankDelta == 0 && absFileDelta > 0;
		return validMoveDirection && IsPathClear(piece, rank, file);
	}
	case PieceType::QUEEN:
	{
		bool validMoveDirection = absRankDelta > 0 && absFileDelta == 0 ||
			absRankDelta == 0 && absFileDelta > 0 ||
			absRankDelta == absFileDelta;
		return validMoveDirection && IsPathClear(piece, rank, file);
	}
	case PieceType::KING:
	{
		return absFileDelta <= 1 && absRankDelta <= 1;
	}
	default:
	{
		return false;
	}
	}
}

void BoardState::ApplyMove(ChessMove* move)
{
	switch (move->type) {
	case ChessMoveType::NORMAL:
		move->piece->rank = move->newRank;
		move->piece->file = move->newFile;
		break;
	case ChessMoveType::CAPTURE:
		move->piece->rank = move->newRank;
		move->piece->file = move->newFile;
		move->extra.captureData.capturedPiece->captured = true;
		break;
	}
	move->piece->hasMoved = true;
}

void BoardState::UnapplyMove(ChessMove* move)
{
	switch (move->type) {
	case ChessMoveType::NORMAL:
		move->piece->rank = move->oldRank;
		move->piece->file = move->oldFile;
		break;
	case ChessMoveType::CAPTURE:
		move->piece->rank = move->oldRank;
		move->piece->file = move->oldFile;
		move->extra.captureData.capturedPiece->captured = false;
		break;
	}

	// if no other moves for this piece exist in the move history, this must have been the first move
	auto otherMove = std::find_if(m_moveHistory.begin(), m_moveHistory.end(),
		[move](const std::unique_ptr<ChessMove>& m) {
			return m.get() != move && m->piece == move->piece;
		});
	if (otherMove == m_moveHistory.end()) {
		move->piece->hasMoved = false;
	}
}

