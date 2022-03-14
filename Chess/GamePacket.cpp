#include "GamePacket.h"
#include <string.h>

void CreateConnectPacket(GamePacket* packet)
{
    memset(packet, 0, sizeof(GamePacket));
    packet->Header.Type = GamePacketType::Connect;
    packet->Header.Length = 0;
}

void CreateAssignColorPacket(GamePacket* packet, Color color)
{
    memset(packet, 0, sizeof(GamePacket));
    packet->Header.Type = GamePacketType::AssignColor;
    packet->Header.Length = sizeof(AssignColorData);
    packet->Data.AssignColor.Color = color;
}

void CreateMovePacket(GamePacket* packet, Rank srcRank, File srcFile, Rank dstRank, File dstFile)
{
    memset(packet, 0, sizeof(GamePacket));
    packet->Header.Type = GamePacketType::Move;
    packet->Header.Length = sizeof(MoveData);
    packet->Data.Move.SrcRank = srcRank;
    packet->Data.Move.SrcFile = srcFile;
    packet->Data.Move.DstRank = dstRank;
    packet->Data.Move.DstFile = dstFile;
    packet->Data.Move.IsPromotion = false;
}

void CreatePromoteMovePacket(GamePacket* packet, Rank srcRank, File srcFile, Rank dstRank, File dstFile, PieceType promoteType)
{
    memset(packet, 0, sizeof(GamePacket));
    packet->Header.Type = GamePacketType::Move;
    packet->Header.Length = sizeof(MoveData);
    packet->Data.Move.SrcRank = srcRank;
    packet->Data.Move.SrcFile = srcFile;
    packet->Data.Move.DstRank = dstRank;
    packet->Data.Move.DstFile = dstFile;
    packet->Data.Move.IsPromotion = true;
    packet->Data.Move.PromotionType = promoteType;
}

void CreateMoveAckPacket(GamePacket* packet, bool accepted)
{
    memset(packet, 0, sizeof(GamePacket));
    packet->Header.Type = GamePacketType::MoveAck;
    packet->Header.Length = sizeof(MoveAckData);
    packet->Data.MoveAck.Accepted = accepted;
}
