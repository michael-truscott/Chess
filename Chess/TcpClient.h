#pragma once

#include "TcpCommon.h"
#include "GamePacket.h"
#include "PacketBuffer.h"

class TcpClient
{
public:
	TcpClient();
	~TcpClient();

	bool Connect(const char* serverName);
	void Shutdown();
	void ReadFromServer();
	void SendMessage(GamePacket* packet);

	void SendMove(Rank srcRank, File srcFile, Rank dstRank, File dstFile);
	void SendPromoteMove(Rank srcRank, File srcFile, Rank dstRank, File dstFile, PieceType promoteType);

	bool IsConnected();
	bool IsMessageReady();
	void GetNextMessage(GamePacket* outPacket);

private:
	bool m_connected;
	IPaddress m_serverAddress;
	TCPsocket m_socket;
	SDLNet_SocketSet m_socketSet;
	PacketBuffer m_buffer;
	uint8_t m_recvBuffer[BUFFER_SIZE];
};

