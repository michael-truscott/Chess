#pragma once

#include <SDL.h>

enum class XboxPadButtons {
	A,
	B,
	X,
	Y,
	L,
	R,
	SELECT,
	START,
	L3,
	R3,
};

namespace Input {
	void Init();
	void Update();
	bool KeyDown(int keyCode);
	bool KeyUp(int keyCode);
	bool KeyPressed(int keyCode);
	bool KeyReleased(int keyCode);

	bool JoyButtonDown(XboxPadButtons button);
	bool JoyButtonUp(XboxPadButtons button);
	bool JoyButtonPressed(XboxPadButtons button);
	bool JoyButtonReleased(XboxPadButtons button);
	
	bool JoyHatDown(Uint8 mask);
	bool JoyHatUp(Uint8 mask);
	bool JoyHatPressed(Uint8 mask);
	bool JoyHatReleased(Uint8 mask);

	// TODO: Get gamepad axis values

	SDL_Point GetMousePos();
	SDL_Point GetMouseDelta();
	bool MouseButtonDown(int button);
	bool MouseButtonUp(int button);
	bool MouseButtonPressed(int button);
	bool MouseButtonReleased(int button);
};