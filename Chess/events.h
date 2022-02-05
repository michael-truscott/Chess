#pragma once

#include <SDL.h>

enum class UserEventType {
	EVENT_GAMEOVER,
	EVENT_MAX,
};

namespace Events {
	void Init();
	Uint32 GetUserEventID(UserEventType type);
}