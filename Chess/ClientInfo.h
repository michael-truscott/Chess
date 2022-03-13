#pragma once

#include "Piece.h"

struct ClientInfo
{
	bool Connected;
	Color Color;
	TCPsocket Socket;
};

