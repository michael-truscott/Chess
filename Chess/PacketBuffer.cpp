#include "PacketBuffer.h"
#include <string.h>
#include <cassert>

PacketBuffer::PacketBuffer() :
	m_buffer(),
	m_bufferCount(0)
{
	for (int i = 0; i < UINT16_MAX; i++)
		m_buffer[i] = i % 0xFF;
}

void PacketBuffer::AddBytes(uint8_t* bytes, int count)
{
	// TODO: probs check buffer bounds
	memcpy(&m_buffer[m_bufferCount], bytes, count);
	m_bufferCount += count;
}

void PacketBuffer::Clear()
{
	memset(m_buffer, 0, UINT16_MAX);
	m_bufferCount = 0;
}

bool PacketBuffer::IsPacketReady() const
{
	if (m_bufferCount < sizeof(GamePacketHeader))
		return false;

	const GamePacket* packet = reinterpret_cast<const GamePacket*>(m_buffer);
	return m_bufferCount >= sizeof(GamePacketHeader) + packet->Header.Length;
}

/// <summary>
/// If a full packet is in the buffer, copy the contents into the given packet pointer
/// and remove it from the front of the buffer.
/// This function can be called repeatedly as long as IsPacketReady returns true.
/// </summary>
/// <param name="outPacket"></param>
void PacketBuffer::ConsumePacket(GamePacket* outPacket)
{
	assert(m_bufferCount >= sizeof(GamePacketHeader));
	GamePacket* packet = reinterpret_cast<GamePacket*>(m_buffer);
	int packetFullLength = sizeof(GamePacketHeader) + packet->Header.Length;
	assert(m_bufferCount >= packetFullLength);
	
	memcpy(outPacket, packet, packetFullLength);
	m_bufferCount -= packetFullLength;
	// if there are any remaining packets in the buffer, move the next one to the front to
	// get picked up by the next 
	if (m_bufferCount > 0)
		memmove(m_buffer, &m_buffer[packetFullLength], m_bufferCount);
}
