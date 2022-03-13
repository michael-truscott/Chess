#include "NetClientChessGameScene.h"
#include <stdexcept>

NetClientChessGameScene::NetClientChessGameScene(SDL_Renderer* renderer, const char* serverName) :
	ChessGameScene::ChessGameScene(renderer),
	m_serverName(serverName),
	m_client(),
	m_pendingMove(),
	m_playerColor(),
	m_gameStarted(false)
{
	m_canUndoMoves = false;
	if (!m_client.Connect(serverName)) {
		std::string error = std::string("Couldn't connect to server ") + serverName;
		throw std::runtime_error(error);
	}
	printf("Connected to server %s\n", serverName);
}

void NetClientChessGameScene::Update(float dt)
{
	m_client.ReadFromServer();
	if (m_client.IsConnected()) {
		GamePacket packet;
		while (m_client.IsMessageReady()) {
			m_client.GetNextMessage(&packet);
			ProcessPacket(&packet);
		}

		if (m_gameStarted)
			ChessGameScene::Update(dt);
	}
	else {
		throw std::runtime_error("Server has disconnected");
	}
}

void NetClientChessGameScene::SelectPiece(Piece* piece)
{
	if (piece == nullptr || piece->color == m_playerColor)
		ChessGameScene::SelectPiece(piece);
}

bool NetClientChessGameScene::TryMovePiece(Piece* piece, Rank rank, File file)
{
	m_pendingMove.isWaiting = true;
	m_pendingMove.piece = piece;
	m_pendingMove.rank = rank;
	m_pendingMove.file = file;
	m_pendingMove.isPromote = false;

	m_client.SendMove(piece->rank, piece->file, rank, file);
	return true;
}

void NetClientChessGameScene::ApplyPromoteMove(PieceType promoteType)
{
	m_pendingMove.isWaiting = true;
	m_pendingMove.piece = m_promotionMove->piece;
	m_pendingMove.rank = m_promotionMove->newRank;
	m_pendingMove.file = m_promotionMove->newFile;
	m_pendingMove.isPromote = true;
	m_pendingMove.promoteType = promoteType;

	m_client.SendPromoteMove(m_promotionMove->piece->rank, m_promotionMove->piece->file, m_promotionMove->newRank, m_promotionMove->newFile, promoteType);
}

void NetClientChessGameScene::ProcessPacket(GamePacket* packet)
{
	switch (packet->Header.Type) {
	case GamePacketType::Connect:
	{
		throw std::runtime_error("Received a Connect message from the server (should never happen)");
		break;
	}
	case GamePacketType::AssignColor:
	{
		if (m_gameStarted)
			throw std::runtime_error("Received an AssignColor message after game has already started");

		const char* colorStr = packet->Data.AssignColor.Color == Color::WHITE ? "white" : "black";
		printf("Server assigned the color %s\n", colorStr);
		m_playerColor = packet->Data.AssignColor.Color;
		if (m_playerColor == Color::BLACK)
			m_flipView = true;
		m_gameStarted = true;
		break;
	}
	case GamePacketType::Move:
	{
		// it's from the server so assume it's legal
		Piece* piece = m_boardState.GetPieceAt(packet->Data.Move.SrcRank, packet->Data.Move.SrcFile);
		if (piece == nullptr)
			throw std::runtime_error("Received an invalid move message from server");

		if (packet->Data.Move.IsPromotion) {
			std::unique_ptr<ChessMove> promoteMove;
			if (!m_boardState.IsMovePromotion(piece, packet->Data.Move.DstRank, packet->Data.Move.DstFile, &promoteMove))
				throw std::runtime_error("Received an invalid promotion move message from server");

			promoteMove->promoteType = packet->Data.Move.PromotionType;
			m_boardState.ApplyPromoteMove(std::move(promoteMove));
		}
		else {
			printf("Server sent move: %s (%c%c) to %c%c\n", PieceTypeToString(piece->type),
				FileToChar(packet->Data.Move.SrcFile), RankToChar(packet->Data.Move.SrcRank),
				FileToChar(packet->Data.Move.DstFile), RankToChar(packet->Data.Move.DstRank));
			m_boardState.MovePiece(piece, packet->Data.Move.DstRank, packet->Data.Move.DstFile);
		}
		break;
	}
	case GamePacketType::MoveAck:
	{
		if (!m_pendingMove.isWaiting)
			throw std::runtime_error("Server sent a MoveAck when there was no pending move on the client side");

		if (m_pendingMove.isPromote) {
			ApplyPromoteMove(m_pendingMove.promoteType);
		}
		else {
			m_boardState.MovePiece(m_pendingMove.piece, m_pendingMove.rank, m_pendingMove.file);
		}
		break;
	}
	default:
		printf("Unexpected packet type received: %d\n", packet->Header.Type);
		break;
	}
}
