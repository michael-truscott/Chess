#pragma once
#include "ChessGameScene.h"
#include "TcpServer.h"

// TODO: extend ChessGameScene to work with a network-connected client playing the other player's moves
// e.g.
// - Wait until client connects before starting game
// - On client connection, send back a TCP message to assign them to the white/black side (at random?)
// - Show message box with failure if client disconnects
// - On making a move, transmit the move over the client TCP connection
// - On receiving a move from the client, check it's a legal move, if so, apply it to the board state and send a MoveAck back
class NetServerChessGameScene : public ChessGameScene
{
public:
	NetServerChessGameScene(SDL_Renderer* renderer);

	virtual void Update(float dt) override;
	virtual void Render() override;

protected:
	virtual bool TryMovePiece(Piece* piece, Rank rank, File file) override;
	virtual void ApplyPromoteMove(PieceType promoteType) override;
	virtual void SelectPiece(Piece* piece) override;

private:
	void ProcessPacket(GamePacket* packet);

	TcpServer m_server;
	Color m_serverColor;
	bool m_gameStarted;
};

