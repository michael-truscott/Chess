#pragma once

#include "ChessGameScene.h"
#include "TcpClient.h"

struct PendingMoveInfo {
	bool isWaiting;
	Piece* piece;
	Rank rank;
	File file;
	bool isPromote;
	PieceType promoteType;
};

class NetClientChessGameScene : public ChessGameScene
{
public:
	NetClientChessGameScene(SDL_Renderer* renderer, const char* serverName);
	virtual void Update(float dt) override;
	virtual void SelectPiece(Piece* piece);
	virtual bool TryMovePiece(Piece* piece, Rank rank, File file) override;
	virtual void ApplyPromoteMove(PieceType promoteType) override;

private:
	void ProcessPacket(GamePacket* packet);

	const char* m_serverName;
	TcpClient m_client;
	Color m_playerColor;
	PendingMoveInfo m_pendingMove;
	bool m_gameStarted;
};

