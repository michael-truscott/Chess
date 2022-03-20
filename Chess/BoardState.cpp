#include "BoardState.h"

BoardState::BoardState() :
	m_currentTurn(Color::WHITE),
	m_currentPlayerInCheck(false),
	m_currentPlayerInCheckmate(false)
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
	return const_cast<Piece*>(const_cast<const BoardState*>(this)->GetPieceAt(rank, file));
}

const Piece* BoardState::GetPieceAt(Rank rank, File file) const
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

bool BoardState::IsMoveLegal(ChessMove* move) const
{
	if (!IsMovePositionLegal(move))
		return false;

	ApplyMove(move);
	bool legal = !IsPositionInCheck(move->piece->color);
	UnapplyMove(move);

	return legal;
}

/// <summary>
/// Check if the prospective move will require the player to select a piece type to promote to
/// </summary>
/// <param name="piece"></param>
/// <param name="newRank"></param>
/// <param name="newFile"></param>
/// <returns></returns>
bool BoardState::IsMovePromotion(Piece* piece, Rank newRank, File newFile, std::unique_ptr<ChessMove>* outMove)
{
	auto move = TryCreateMove(piece, newRank, newFile);
	if (move && IsMoveLegal(move.get()) && move->isPromotion) {
		// sorry future Michael
		(*outMove) = std::move(move);
		return true;
	}
	return false;
}

/// <summary>
/// Returns true if the given piece is a pawn that just moved 2 squares forward on the previous turn.
/// </summary>
/// <param name="piece"></param>
/// <returns></returns>
bool BoardState::IsEnPassantPossible(const Piece* piece) const
{
	if (piece->captured || piece->type != PieceType::PAWN)
		return false;

	if (m_moveHistory.empty())
		return false;

	ChessMove* lastMove = m_moveHistory.back().get();
	if (lastMove->piece != piece)
		return false;

	int rankDelta = std::abs((int)lastMove->newRank - (int)lastMove->oldRank);
	return rankDelta == 2;
}

void BoardState::ApplyPromoteMove(std::unique_ptr<ChessMove> move)
{
	ApplyMove(move.get());
	m_moveHistory.push_back(std::move(move));
	NextTurn();
}

void BoardState::RemovePiece(Piece* piece)
{
	piece->captured = true;
}

void BoardState::NextTurn()
{
	m_currentTurn = m_currentTurn == Color::WHITE ? Color::BLACK : Color::WHITE;
	m_currentPlayerInCheck = IsPositionInCheck(m_currentTurn);
	m_currentPlayerInCheckmate = IsPositionInCheckmate(m_currentTurn);
}

/// <summary>
/// Try and create a ChessMove instance representing the given move for the piece.
/// Returns a unique_ptr holding the move if successful, or holding null if not.
/// NOTE: This function can generate illegal moves, IsLegalMove() is responsible for filtering them out.
/// </summary>
/// <param name="piece">The piece to move.</param>
/// <param name="newRank"></param>
/// <param name="newFile"></param>
/// <returns></returns>
std::unique_ptr<ChessMove> BoardState::TryCreateMove(Piece* piece, Rank newRank, File newFile) const
{
	// basic move info
	auto move = std::make_unique<ChessMove>();
	move->type = ChessMoveType::NORMAL;
	move->piece = piece;
	move->oldRank = piece->rank;
	move->oldFile = piece->file;
	move->newRank = newRank;
	move->newFile = newFile;

	// special cases go here:
	// is it a capture?
	const Piece* capturedPiece = GetPieceAt(newRank, newFile);
	if (capturedPiece) {
		if (capturedPiece->color == piece->color) { // filter out some silly cases
			return nullptr;
		}
		move->type = ChessMoveType::CAPTURE;
		move->extra.captureData.capturedPiece = const_cast<Piece*>(capturedPiece);
	}

	// is it an en passant?
	int enPassantRankOffset = piece->color == Color::WHITE ? -1 : 1;
	const Piece* pieceBehindTargetSquare = GetPieceAt((Rank)((int)newRank + enPassantRankOffset), newFile);
	if (piece->type == PieceType::PAWN && move->type != ChessMoveType::CAPTURE &&
		pieceBehindTargetSquare != nullptr && IsEnPassantPossible(pieceBehindTargetSquare))
	{
		move->type = ChessMoveType::CAPTURE;
		move->extra.captureData.capturedPiece = const_cast<Piece*>(pieceBehindTargetSquare);
		move->extra.captureData.enPassant = true;
	}

	// is it a castle?
	if (piece->type == PieceType::KING && !piece->hasMoved &&
		(newRank == Rank::R1 || newRank == Rank::R8) &&
		(newFile == File::C || newFile == File::G))
	{
		const Piece* rook = nullptr;
		if (newFile == File::C)
			rook = GetPieceAt(piece->rank, File::A);
		else if (newFile == File::G)
			rook = GetPieceAt(piece->rank, File::H);
		if (rook && !rook->hasMoved) {
			move->type = ChessMoveType::CASTLE;
			move->extra.castleData.rook = const_cast<Piece*>(rook);
			move->extra.castleData.rookOldRank = rook->rank;
			move->extra.castleData.rookOldFile = rook->file;
		}
	}

	// is it a promotion?
	if (piece->type == PieceType::PAWN &&
		(piece->color == Color::WHITE && newRank == Rank::R8 || piece->color == Color::BLACK && newRank == Rank::R1))
	{
		move->isPromotion = true;
		move->promoteType = PieceType::QUEEN;
	}
	return move;
}

bool BoardState::IsPathClear(Piece* piece, Rank targetRank, File targetFile) const
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

// Is the given square currently under attack by any piece of a certain colour?
bool BoardState::IsSquareUnderAttackByColor(Rank rank, File file, Color color) const
{
	for (auto& piece : m_pieces) {
		if (piece->captured || piece->color != color)
			continue;

		if (IsPieceAttackingSquare(piece.get(), rank, file))
			return true;
	}
	return false;
}

// Is the given piece currently attacking the given square?
bool BoardState::IsPieceAttackingSquare(Piece* piece, Rank rank, File file) const
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

bool BoardState::IsPositionInCheck(Color color) const
{
	auto king = std::find_if(m_pieces.begin(), m_pieces.end(), [color](const std::unique_ptr<Piece>& piece) {
		return piece->type == PieceType::KING && piece->color == color;
	});
	if (king == m_pieces.end()) // king doesn't exist? this probably isn't good
		return false;

	return IsSquareUnderAttackByColor((*king)->rank, (*king)->file, color == Color::WHITE ? Color::BLACK : Color::WHITE);
}

bool BoardState::IsPositionInCheckmate(Color color) const
{
	for (auto& piece : m_pieces) {
		if (piece->color == color && !GetAllLegalMovesForPiece(piece.get()).empty())
			return false;
	}
	return true;
}

const ChessMove* BoardState::LastMove() const
{
	if (m_moveHistory.empty())
		return nullptr;

	return m_moveHistory.back().get();
}

std::vector<std::unique_ptr<ChessMove>> BoardState::GetAllLegalMovesForPiece(Piece* piece) const
{
	std::vector<std::unique_ptr<ChessMove>> result;
	if (piece->captured)
		return result;

	for (int rank = (int)Rank::R1; rank <= (int)Rank::R8; rank++) {
		for (int file = (int)File::A; file <= (int)File::H; file++) {
			// moving to the same square should never be a legal move but skip it just in case
			if (rank == (int)piece->rank && file == (int)piece->file)
				continue;

			auto move = TryCreateMove(piece, (Rank)rank, (File)file);
			if (move && IsMoveLegal(move.get()))
				result.push_back(std::move(move));
		}
	}
	return result;
}

const bool BoardState::CurrentPlayerInCheck() const
{
	return m_currentPlayerInCheck;
}

const bool BoardState::CurrentPlayerInCheckmate() const
{
	return m_currentPlayerInCheckmate;
}

bool BoardState::IsMovePositionLegal(ChessMove* move) const
{
	int rankDelta = (int)move->newRank - (int)move->oldRank;
	int fileDelta = (int)move->newFile - (int)move->oldFile;
	int absRankDelta = std::abs(rankDelta);
	int absFileDelta = std::abs(fileDelta);

	switch (move->piece->type) {
	case PieceType::PAWN:
	{
		int oneSpace = move->piece->color == Color::WHITE ? 1 : -1;
		int twoSpaces = move->piece->color == Color::WHITE ? 2 : -2;
		if (move->type == ChessMoveType::CAPTURE) {
			// TODO: double check en passant is legal?
			if (rankDelta == oneSpace && absFileDelta == 1)
				return true;
		}
		else {
			if (fileDelta == 0 && rankDelta == oneSpace)
				return true;
			if (!move->piece->hasMoved && fileDelta == 0 && rankDelta == twoSpaces && IsPathClear(move->piece, move->newRank, move->newFile))
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
		if (absRankDelta == absFileDelta && IsPathClear(move->piece, move->newRank, move->newFile))
			return true;
		break;
	}
	case PieceType::ROOK:
	{
		if ((absRankDelta == 0 && absFileDelta != 0 ||
			absRankDelta != 0 && absFileDelta == 0) &&
			IsPathClear(move->piece, move->newRank, move->newFile))
			return true;
		break;
	}
	case PieceType::QUEEN:
		if ((absRankDelta == 0 && absFileDelta != 0 ||
			absRankDelta != 0 && absFileDelta == 0 ||
			absRankDelta == absFileDelta) &&
			IsPathClear(move->piece, move->newRank, move->newFile))
			return true;
		break;
	case PieceType::KING:
		if (absFileDelta <= 1 && absRankDelta <= 1)
			return true;

		if (move->type == ChessMoveType::CASTLE) {
			if (move->piece->hasMoved || !move->extra.castleData.rook || move->extra.castleData.rook->hasMoved || move->newRank != move->oldRank)
				return false;

			if (IsPositionInCheck(move->piece->color))
				return false;

			if ((move->newFile == File::G || move->newFile == File::C) &&
				IsPathClear(move->piece, move->extra.castleData.rook->rank, move->extra.castleData.rook->file))
				return true;
		}
		break;
	}
	return false;
}

void BoardState::ApplyMove(ChessMove* move)
{
	switch (move->type) {
	case ChessMoveType::NORMAL:
		move->piece->rank = move->newRank;
		move->piece->file = move->newFile;
		if (move->isPromotion)
			move->piece->type = move->promoteType;
		break;
	case ChessMoveType::CAPTURE:
		move->piece->rank = move->newRank;
		move->piece->file = move->newFile;
		if (move->isPromotion)
			move->piece->type = move->promoteType;
		move->extra.captureData.capturedPiece->captured = true;
		break;
	case ChessMoveType::CASTLE:
		move->piece->rank = move->newRank;
		move->piece->file = move->newFile;
		move->extra.castleData.rook->rank = move->newRank;
		move->extra.castleData.rook->file = move->newFile == File::G ? File::F : File::D;
		move->extra.castleData.rook->hasMoved = true;
	}
	move->piece->hasMoved = true;
}

void BoardState::UnapplyMove(ChessMove* move)
{
	switch (move->type) {
	case ChessMoveType::NORMAL:
		move->piece->rank = move->oldRank;
		move->piece->file = move->oldFile;
		if (move->isPromotion)
			move->piece->type = PieceType::PAWN;
		break;
	case ChessMoveType::CAPTURE:
		move->piece->rank = move->oldRank;
		move->piece->file = move->oldFile;
		if (move->isPromotion)
			move->piece->type = PieceType::PAWN;
		move->extra.captureData.capturedPiece->captured = false;
		break;
	case ChessMoveType::CASTLE:
		move->piece->rank = move->oldRank;
		move->piece->file = move->oldFile;
		move->extra.castleData.rook->rank = move->extra.castleData.rookOldRank;
		move->extra.castleData.rook->file = move->extra.castleData.rookOldFile;
		move->extra.castleData.rook->hasMoved = false;
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

void BoardState::ApplyMove(ChessMove* move) const
{
	const_cast<BoardState*>(this)->ApplyMove(move);
}

void BoardState::UnapplyMove(ChessMove* move) const
{
	const_cast<BoardState*>(this)->UnapplyMove(move);
}