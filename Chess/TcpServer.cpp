#include "TcpServer.h"
#include <SDL_net.h>

TcpServer::TcpServer() :
	m_client(),
	m_address(),
	m_listenSocket(),
	m_recvBuffer(),
	m_socketSet()
{
}

bool TcpServer::Init()
{
	if (SDLNet_ResolveHost(&m_address, nullptr, SERVER_PORT) != 0) {
		printf("Failed to resolve host: %s\n", SDLNet_GetError());
		return false;
	}

	m_listenSocket = SDLNet_TCP_Open(&m_address);
	if (m_listenSocket == nullptr) {
		printf("Failed to open server port: %s\n", SDLNet_GetError());
		return false;
	}

	m_socketSet = SDLNet_AllocSocketSet(2); // 1 for listen socket, 1 for client
	if (m_socketSet == nullptr) {
		printf("Failed to allocate socket set: %s\n", SDLNet_GetError());
		return false;
	}

	if (SDLNet_TCP_AddSocket(m_socketSet, m_listenSocket) == -1) {
		printf("Failed to add socket to set: %s\n", SDLNet_GetError());
		return false;
	}

	return true;
}

void TcpServer::Shutdown()
{
	if (SDLNet_TCP_DelSocket(m_socketSet, m_listenSocket) == -1) {
		printf("Error deleting server socket: %s\n", SDLNet_GetError());
	}
	SDLNet_TCP_Close(m_listenSocket);

	if (m_client.Connected) {
		SDLNet_TCP_DelSocket(m_socketSet, m_client.Socket);
		SDLNet_TCP_Close(m_client.Socket);
	}

	SDLNet_FreeSocketSet(m_socketSet);
}

void TcpServer::ProcessSockets()
{
	int available = SDLNet_CheckSockets(m_socketSet, 0);

	if (available == -1) {
		printf("Error in SDLNet_CheckSockets: %s\n", SDLNet_GetError());
		// todo: throw error
		return;
	}

	if (available > 0) {
		if (SDLNet_SocketReady(m_listenSocket)) {
			if (IsClientConnected()) {
				printf("Client sockets are full, rejecting connection\n");
			}
			else {
				printf("Accepting client connection\n");
				m_client.Socket = SDLNet_TCP_Accept(m_listenSocket);
				m_client.Connected = true;
				SDLNet_TCP_AddSocket(m_socketSet, m_client.Socket);
			}
		}

		if (IsClientConnected() && SDLNet_SocketReady(m_client.Socket)) {
			int bytesReceived = SDLNet_TCP_Recv(m_client.Socket, m_recvBuffer, BUFFER_SIZE);
			if (bytesReceived <= 0) {
				printf("Connection closed by client\n\n");
				ResetClient();
			}
			else {
				m_buffer.AddBytes(m_recvBuffer, bytesReceived);
			}
		}
	}
}

bool TcpServer::IsClientConnected()
{
	return m_client.Connected;
}

bool TcpServer::IsClientMessageReady()
{
	return m_buffer.IsPacketReady();
}

void TcpServer::GetNextClientMessage(GamePacket* outPacket)
{
	m_buffer.ConsumePacket(outPacket);
}

void TcpServer::AssignClientColor(Color color)
{
	GamePacket packet;
	memset(&packet, 0, sizeof(packet));
	packet.Header.Type = GamePacketType::AssignColor;
	packet.Header.Length = sizeof(AssignColorData);
	packet.Data.AssignColor.Color = color;
	SendToClient(&packet);

	m_client.Color = color;
	
}

Color TcpServer::GetClientColor()
{
	return m_client.Color;
}

void TcpServer::SendMoveAckToClient(bool accepted)
{
	GamePacket packet;
	memset(&packet, 0, sizeof(packet));
	packet.Header.Type = GamePacketType::MoveAck;
	packet.Header.Length = sizeof(MoveAckData);
	packet.Data.MoveAck.Accepted = accepted;
	SendToClient(&packet);
}

void TcpServer::SendMoveToClient(Rank srcRank, File srcFile, Rank dstRank, File dstFile)
{
	GamePacket packet;
	memset(&packet, 0, sizeof(packet));
	packet.Header.Type = GamePacketType::Move;
	packet.Header.Length = sizeof(MoveData);
	packet.Data.Move.SrcRank = srcRank;
	packet.Data.Move.SrcFile = srcFile;
	packet.Data.Move.DstRank = dstRank;
	packet.Data.Move.DstFile = dstFile;
	packet.Data.Move.IsPromotion = false;
	SendToClient(&packet);
}

void TcpServer::SendPromoteMoveToClient(Rank srcRank, File srcFile, Rank dstRank, File dstFile, PieceType promoteType)
{
	GamePacket packet;
	memset(&packet, 0, sizeof(packet));
	packet.Header.Type = GamePacketType::Move;
	packet.Header.Length = sizeof(MoveData);
	packet.Data.Move.SrcRank = srcRank;
	packet.Data.Move.SrcFile = srcFile;
	packet.Data.Move.DstRank = dstRank;
	packet.Data.Move.DstFile = dstFile;
	packet.Data.Move.IsPromotion = true;
	packet.Data.Move.PromotionType = promoteType;
	SendToClient(&packet);
}

void TcpServer::SendToClient(GamePacket* packet)
{
	if (!IsClientConnected())
		return;

	int bytesToSend = sizeof(GamePacketHeader) + packet->Header.Length;
	int bytesSent = SDLNet_TCP_Send(m_client.Socket, packet, bytesToSend);
	if (bytesSent < bytesToSend) {
		printf("Failed to send packet to client\n");
		ResetClient();
	}
}

void TcpServer::ResetClient()
{
	if (m_client.Connected) {
		m_client.Connected = false;
		SDLNet_TCP_DelSocket(m_socketSet, m_client.Socket);
		SDLNet_TCP_Close(m_client.Socket);
	}
}
