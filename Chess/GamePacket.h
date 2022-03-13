#pragma once

#include <cstdint>
#include "Piece.h"

enum class GamePacketType : uint8_t {
	Connect,
	AssignColor,
	Move,
	MoveAck,
};

#pragma pack(push, r1, 1)

struct GamePacketHeader {
	GamePacketType Type;
	uint16_t Length;
};


// ===================================================================
// Packet Types
// ===================================================================
struct AssignColorData {
	Color Color;
};

// client will send the simplest possible definition of a move,
// any logic such as captures/castling/en passant will be calculated
// on the receiving end and the receiver will indicate acceptance/rejection
// of the move with an Ack packet.
struct MoveData {
	Rank SrcRank;
	File SrcFile;
	Rank DstRank;
	File DstFile;
	bool IsPromotion;
	PieceType PromotionType;
};

struct MoveAckData {
	bool Accepted;
};

struct GamePacketData {
	union {
		AssignColorData AssignColor;
		MoveData Move;
		MoveAckData MoveAck;
	};
};

struct GamePacket {
	GamePacketHeader Header;
	GamePacketData Data;
};

void CreateConnectPacket(GamePacket* packet);
void CreateAssignColorPacket(GamePacket* packet, Color color);
void CreateMovePacket(GamePacket* packet, Rank srcRank, File srcFile, Rank dstRank, File dstFile);
void CreatePromoteMovePacket(GamePacket* packet, Rank srcRank, File srcFile, Rank dstRank, File dstFile, PieceType promoteType);
void CreateMoveAckPacket(GamePacket* packet, bool accepted);

#pragma pack(pop, r1)
