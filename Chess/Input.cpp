#include "Input.h"
#include <SDL.h>
#include <cstring>
#include <memory>
#include <iostream>

namespace Input {

	// Anonymous namespace to hide implmentation details
	namespace {
		std::unique_ptr<Uint8[]> keyBuffer;
		std::unique_ptr<Uint8[]> prevKeyBuffer;
		int keyBufferSize;

		SDL_Joystick* joystick;

		int joystickButtonCount;
		std::unique_ptr<Uint8[]> joystickButtons;
		std::unique_ptr<Uint8[]> prevJoystickButtons;

		int joystickAxisCount;

		Uint8 joystickHat, prevJoystickHat;

		Uint32 mouseState, prevMouseState;
		int mouseX, mouseY, prevMouseX, prevMouseY;
	}

	void Init()
	{
		const Uint8* tempBuf = SDL_GetKeyboardState(&keyBufferSize);
		keyBuffer = std::unique_ptr<Uint8[]>(new Uint8[keyBufferSize]);
		prevKeyBuffer = std::unique_ptr<Uint8[]>(new Uint8[keyBufferSize]);
		memcpy(keyBuffer.get(), tempBuf, keyBufferSize);
		memcpy(prevKeyBuffer.get(), tempBuf, keyBufferSize);

		if (SDL_NumJoysticks() > 0) {
			joystick = SDL_JoystickOpen(0);
			const char* joyName = SDL_JoystickName(joystick);
			std::cout << "Joystick: " << joyName << std::endl;

			joystickAxisCount = SDL_JoystickNumAxes(joystick);

			joystickButtonCount = SDL_JoystickNumButtons(joystick);
			joystickButtons = std::unique_ptr<Uint8[]>(new Uint8[joystickButtonCount]);
			prevJoystickButtons = std::unique_ptr<Uint8[]>(new Uint8[joystickButtonCount]);

			for (int i = 0; i < joystickButtonCount; i++) {
				if (SDL_JoystickNumHats(joystick) > 0) {
					joystickHat = SDL_JoystickGetHat(joystick, 0);
					prevJoystickHat = joystickHat;
				}
				SDL_JoystickGetHat(joystick, 0);
				joystickButtons[i] = SDL_JoystickGetButton(joystick, i);
				prevJoystickButtons[i] = SDL_JoystickGetButton(joystick, i);
			}
			SDL_JoystickEventState(SDL_QUERY);
		}

		mouseState = SDL_GetMouseState(&mouseX, &mouseY);
		prevMouseState = mouseState;
	}

	void Update() {
		memcpy(prevKeyBuffer.get(), keyBuffer.get(), keyBufferSize);

		const Uint8* tempBuf = SDL_GetKeyboardState(&keyBufferSize);
		memcpy(keyBuffer.get(), tempBuf, keyBufferSize);

		SDL_JoystickUpdate();
		memcpy(prevJoystickButtons.get(), joystickButtons.get(), joystickButtonCount);
		for (int i = 0; i < joystickButtonCount; i++) {
			joystickButtons[i] = SDL_JoystickGetButton(joystick, i);
		}
		prevJoystickHat = joystickHat;
		joystickHat = SDL_JoystickGetHat(joystick, 0);

		prevMouseState = mouseState;
		prevMouseX = mouseX;
		prevMouseY = mouseY;
		mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	}

	bool KeyDown(int keyCode)
	{
		return keyBuffer[keyCode];
	}
	bool KeyUp(int keyCode)
	{
		return !keyBuffer[keyCode];
	}
	bool KeyPressed(int keyCode)
	{
		return keyBuffer[keyCode] && !prevKeyBuffer[keyCode];
	}
	bool KeyReleased(int keyCode)
	{
		return !keyBuffer[keyCode] && prevKeyBuffer[keyCode];
	}

	bool JoyButtonDown(XboxPadButtons button)
	{
		return joystick && joystickButtons[(int)button];
	}
	bool JoyButtonUp(XboxPadButtons button)
	{
		return joystick && !joystickButtons[(int)button];
	}
	bool JoyButtonPressed(XboxPadButtons button)
	{
		return joystick && joystickButtons[(int)button] && !prevJoystickButtons[(int)button];
	}
	bool JoyButtonReleased(XboxPadButtons button)
	{
		return joystick && !joystickButtons[(int)button] && prevJoystickButtons[(int)button];
	}
	bool JoyHatDown(Uint8 mask)
	{
		return joystick && (joystickHat & mask);
	}
	bool JoyHatUp(Uint8 mask)
	{
		return joystick && ((joystickHat & mask) == 0);
	}
	bool JoyHatPressed(Uint8 mask)
	{
		return joystick && (joystickHat & mask) && ((prevJoystickHat & mask) == 0);
	}
	bool JoyHatReleased(Uint8 mask)
	{
		return joystick && ((joystickHat & mask) == 0) && (prevJoystickHat & mask);
	}

	SDL_Point GetMousePos()
	{
		return SDL_Point{ mouseX, mouseY };
	}
	SDL_Point GetMouseDelta()
	{
		return SDL_Point{ mouseX - prevMouseX, mouseY - prevMouseY };
	}
	bool MouseButtonDown(int button)
	{
		return (mouseState & SDL_BUTTON(button)) != 0;
	}
	bool MouseButtonUp(int button)
	{
		return (mouseState & SDL_BUTTON(button)) == 0;
	}
	bool MouseButtonPressed(int button)
	{
		return (mouseState & SDL_BUTTON(button)) != 0 && (prevMouseState & SDL_BUTTON(button)) == 0;
	}
	bool MouseButtonReleased(int button)
	{
		return (mouseState & SDL_BUTTON(button)) == 0 && (prevMouseState & SDL_BUTTON(button)) != 0;
	}
}
