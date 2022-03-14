#include "TcpClient.h"
#include <stdio.h>

TcpClient::TcpClient() :
	m_serverAddress(),
	m_socket(),
	m_socketSet(),
	m_recvBuffer(),
	m_connected(false)
{
}

TcpClient::~TcpClient()
{
	Shutdown();
}

bool TcpClient::Connect(const char* serverName)
{
	if (SDLNet_ResolveHost(&m_serverAddress, serverName, SERVER_PORT) != 0) {
		printf("Failed to resolve host %s:%u: %s\n", serverName, SERVER_PORT, SDLNet_GetError());
		return false;
	}

	m_socket = SDLNet_TCP_Open(&m_serverAddress);
	if (!m_socket) {
		printf("Failed to connect to %s:%u: %s\n", serverName, SERVER_PORT, SDLNet_GetError());
		return false;
	}

	m_socketSet = SDLNet_AllocSocketSet(1);
	if (m_socketSet == nullptr) {
		printf("Failed to allocate socket set: %s\n", SDLNet_GetError());
		return false;
	}

	if (SDLNet_TCP_AddSocket(m_socketSet, m_socket) == -1) {
		printf("Failed to add socket to set: %s\n", SDLNet_GetError());
		return false;
	}

	m_connected = true;

	GamePacket connectPacket;
	CreateConnectPacket(&connectPacket);
	SendMessage(&connectPacket);
    return true;
}

void TcpClient::Shutdown()
{
	if (m_socket != NULL) {
		if (SDLNet_TCP_DelSocket(m_socketSet, m_socket) == -1) {
			printf("Error deleting client socket: %s\n", SDLNet_GetError());
		}
		SDLNet_TCP_Close(m_socket);
	}
	
	SDLNet_FreeSocketSet(m_socketSet);
}

void TcpClient::ReadFromServer()
{
	int available = SDLNet_CheckSockets(m_socketSet, 0);
	if (available == 0)
		return;

	if (SDLNet_SocketReady(m_socket)) {
		int bytesReceived = SDLNet_TCP_Recv(m_socket, m_recvBuffer, BUFFER_SIZE);
		if (bytesReceived <= 0) {
			printf("Connection closed by server\n\n");
			m_connected = false;
		}
		else {
			m_buffer.AddBytes(m_recvBuffer, bytesReceived);
		}
	}
}

void TcpClient::SendMessage(GamePacket* packet)
{
	if (!m_connected)
		return;

	int bytesToSend = sizeof(GamePacketHeader) + packet->Header.Length;
	int bytesSent = SDLNet_TCP_Send(m_socket, packet, bytesToSend);
	if (bytesSent < bytesToSend) {
		printf("Failed to send packet\n");
		m_connected = false;
	}
}

void TcpClient::SendMove(Rank srcRank, File srcFile, Rank dstRank, File dstFile)
{
	GamePacket packet;
	CreateMovePacket(&packet, srcRank, srcFile, dstRank, dstFile);
	SendMessage(&packet);
}

void TcpClient::SendPromoteMove(Rank srcRank, File srcFile, Rank dstRank, File dstFile, PieceType promoteType)
{
	GamePacket packet;
	CreatePromoteMovePacket(&packet, srcRank, srcFile, dstRank, dstFile, promoteType);
	SendMessage(&packet);
}

bool TcpClient::IsConnected()
{
	return m_connected;
}

bool TcpClient::IsMessageReady()
{
	return m_buffer.IsPacketReady();
}

void TcpClient::GetNextMessage(GamePacket* outPacket)
{
	m_buffer.ConsumePacket(outPacket);
}
