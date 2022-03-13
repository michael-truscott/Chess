#pragma once

#include <SDL.h>
#include "SceneManager.h"

class Game
{
public:
	Game(int argc, char** argv);
	~Game();

	void Run();

private:
	void Init();
	void Shutdown();

	void Update(float dt);
	void Render();

	int m_argc;
	char** m_argv;

	bool m_finished;
	Uint64 m_prevTime;
	float m_currentFrameTime;

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_frameBuffer;

	SceneManager m_sceneManager;
};
