#pragma once
#include "ChessGameScene.h"

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
};

