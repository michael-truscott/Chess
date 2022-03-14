#include "Events.h"
#include <stdexcept>

namespace Events {

	namespace {
		Uint32 firstUserEvent = (Uint32)-1;
	}

	void Init()
	{
		firstUserEvent = SDL_RegisterEvents((int)UserEventType::EVENT_MAX);
		if (firstUserEvent == (Uint32)-1) {
			throw std::runtime_error("SDL_RegisterEvents failed");
		}
	}

	Uint32 GetUserEventID(UserEventType type)
	{
		if (firstUserEvent == (Uint32)-1) {
			throw std::invalid_argument("User events have not been registered, call Event::Init() first");
		}
		return firstUserEvent + (int)type;
	}
}
