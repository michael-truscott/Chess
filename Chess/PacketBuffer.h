#pragma once

#include <cstdint>
#include "GamePacket.h"

class PacketBuffer
{
public:
	PacketBuffer();
	void AddBytes(uint8_t* bytes, int count);
	void Clear();
	bool IsPacketReady() const;
	void ConsumePacket(GamePacket* outPacket);

private:
	uint8_t m_buffer[UINT16_MAX];
	uint16_t m_bufferCount;
};

