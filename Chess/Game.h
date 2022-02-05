#pragma once

#include <SDL.h>
#include "SceneManager.h"

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	void Init();
	void Shutdown();

	void Update(float dt);
	void Render();

	bool m_finished;
	Uint64 m_prevTime;
	float m_currentFrameTime;

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_frameBuffer;

	SceneManager m_sceneManager;
};
