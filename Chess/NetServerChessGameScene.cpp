#include "NetServerChessGameScene.h"
#include <stdexcept>

NetServerChessGameScene::NetServerChessGameScene(SDL_Renderer* renderer) :
	ChessGameScene::ChessGameScene(renderer),
	m_serverColor(),
	m_gameStarted(false)
{
	m_canUndoMoves = false;
	m_server.Init();
}

void NetServerChessGameScene::Update(float dt)
{
	m_server.ProcessSockets();
	if (m_server.IsClientConnected()) {
		GamePacket packet;
		while (m_server.IsClientMessageReady()) {
			m_server.GetNextClientMessage(&packet);
			ProcessPacket(&packet);
		}

		if (m_gameStarted)
			ChessGameScene::Update(dt);
	}
	else {
		if (m_gameStarted)
			throw std::runtime_error("Client has disconnected");
	}
}

void NetServerChessGameScene::Render()
{
	ChessGameScene::Render();
	if (!m_server.IsClientConnected()) {
		// TODO: draw "waiting for client connection" message
	}
}

bool NetServerChessGameScene::TryMovePiece(Piece* piece, Rank rank, File file)
{
	Rank oldRank = piece->rank;
	File oldFile = piece->file;
	if (ChessGameScene::TryMovePiece(piece, rank, file)) {
		m_server.SendMoveToClient(oldRank, oldFile, rank, file);
		return true;
	}
	else {
		return false;
	}
}

void NetServerChessGameScene::ApplyPromoteMove(PieceType promoteType)
{
	m_server.SendPromoteMoveToClient(m_promotionMove->oldRank, m_promotionMove->oldFile,
		m_promotionMove->newRank, m_promotionMove->newFile,
		promoteType);
	ChessGameScene::ApplyPromoteMove(promoteType);
}

void NetServerChessGameScene::SelectPiece(Piece* piece)
{
	if (piece == nullptr || piece->color == m_serverColor)
		ChessGameScene::SelectPiece(piece);
}

void NetServerChessGameScene::ProcessPacket(GamePacket* packet)
{
	switch (packet->Header.Type) {
	case GamePacketType::Connect:
	{
		if (m_gameStarted) {
			throw std::runtime_error("Received a Connect message after client has already established connection");
		}
		
		m_gameStarted = true;
		m_serverColor = rand() % 2 == 0 ? Color::WHITE : Color::BLACK;
		if (m_serverColor == Color::BLACK) {
			m_flipView = true;
		}
		m_server.AssignClientColor(m_serverColor == Color::WHITE ? Color::BLACK : Color::WHITE);
		break;
	}
	case GamePacketType::Move:
	{
		if (packet->Header.Length != sizeof(MoveData)) {
			printf("Invalid move packet length: %d (expected %d)\n", packet->Header.Length, (int)sizeof(MoveData));
			m_server.SendMoveAckToClient(false);
			break;
		}
		Piece* piece = m_boardState.GetPieceAt(packet->Data.Move.SrcRank, packet->Data.Move.SrcFile);
		if (piece == nullptr) {
			printf("Invalid move: no piece at %c%c\n", FileToChar(packet->Data.Move.SrcFile), RankToChar(packet->Data.Move.SrcRank));
			m_server.SendMoveAckToClient(false);
			break;
		}
		if (piece->color != m_server.GetClientColor()) {
			printf("Invalid move: target piece %s (%c%c) does not belong to client\n", PieceTypeToString(piece->type),
				FileToChar(packet->Data.Move.SrcFile), RankToChar(packet->Data.Move.SrcRank));
			m_server.SendMoveAckToClient(false);
			break;
		}
		if (m_boardState.CurrentTurn() != m_server.GetClientColor()) {
			printf("Invalid move: out of turn\n");
			m_server.SendMoveAckToClient(false);
			break;
		}

		if (packet->Data.Move.IsPromotion) {
			std::unique_ptr<ChessMove> promoteMove;
			if (!m_boardState.IsMovePromotion(piece, packet->Data.Move.DstRank, packet->Data.Move.DstFile, &promoteMove)) {
				printf("Invalid move: illegal promotion\n");
				m_server.SendMoveAckToClient(false);
				break;
			}

			promoteMove->promoteType = packet->Data.Move.PromotionType;
			m_boardState.ApplyPromoteMove(std::make_unique<ChessMove>());
			printf("Client sent move: %s (%c%c) to %c%c\n", PieceTypeToString(piece->type),
				FileToChar(packet->Data.Move.SrcFile), RankToChar(packet->Data.Move.SrcRank),
				FileToChar(packet->Data.Move.DstFile), RankToChar(packet->Data.Move.DstRank));
			m_server.SendMoveAckToClient(true);
			break;
		}
		else {
			// non-promote move
			if (m_boardState.MovePiece(piece, packet->Data.Move.DstRank, packet->Data.Move.DstFile))
			{
				printf("Client sent move: %s (%c%c) to %c%c\n", PieceTypeToString(piece->type),
					FileToChar(packet->Data.Move.SrcFile), RankToChar(packet->Data.Move.SrcRank),
					FileToChar(packet->Data.Move.DstFile), RankToChar(packet->Data.Move.DstRank));
				m_server.SendMoveAckToClient(true);
				break;
			}
			else {
				printf("Invalid move: %s (%c%c) to %c%c\n", PieceTypeToString(piece->type),
					FileToChar(packet->Data.Move.SrcFile), RankToChar(packet->Data.Move.SrcRank),
					FileToChar(packet->Data.Move.DstFile), RankToChar(packet->Data.Move.DstRank));
				m_server.SendMoveAckToClient(false);
				break;
			}
		}
		break;
	}
	default:
		printf("Unexpected packet type received: %d\n", packet->Header.Type);
		break;
	}
}
