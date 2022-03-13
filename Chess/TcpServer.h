#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include "TcpCommon.h"
#include "ClientInfo.h"
#include "GamePacket.h"
#include "PacketBuffer.h"

class TcpServer
{
public:
	TcpServer();
	~TcpServer();

	bool Init();
	void Shutdown();

	void ProcessSockets();

	bool IsClientConnected();
	bool IsClientMessageReady();
	void GetNextClientMessage(GamePacket* outPacket);

	void AssignClientColor(Color color);
	Color GetClientColor();

	void SendMoveAckToClient(bool accepted);
	void SendMoveToClient(Rank srcRank, File srcFile, Rank dstRank, File dstFile);
	void SendPromoteMoveToClient(Rank srcRank, File srcFile, Rank dstRank, File dstFile, PieceType promoteType);

	void SendToClient(GamePacket* packet);

private:
	void ResetClient();

	ClientInfo m_client;
	PacketBuffer m_buffer;
	uint8_t m_recvBuffer[BUFFER_SIZE];

	IPaddress m_address;
	TCPsocket m_listenSocket;
	SDLNet_SocketSet m_socketSet;
};

