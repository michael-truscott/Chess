#include "Game.h"
#include <stdexcept>
#include <SDL_image.h>
#include <SDL_net.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include "globals.h"
#include "Input.h"
#include "AssetLoader.h"
#include "ChessGameScene.h"
#include "NetServerChessGameScene.h"
#include "Events.h"

static constexpr int FRAMES_PER_SEC = 60;
static constexpr float FRAME_PERIOD = 1 / (float)FRAMES_PER_SEC;

Game::Game(int argc, char** argv) :
	m_argc(argc),
	m_argv(argv),
	m_finished(false),
	m_prevTime(0),
	m_currentFrameTime(0),
	m_window(nullptr),
	m_renderer(nullptr),
	m_frameBuffer(nullptr),
	m_sceneManager()
{
	Init();
}

Game::~Game()
{
	Shutdown();
}

void Game::Run()
{
	m_prevTime = SDL_GetPerformanceCounter();
	while (!m_finished)
	{
		Uint64 currentTime = SDL_GetPerformanceCounter();
		float dt = (currentTime - m_prevTime) / (float)SDL_GetPerformanceFrequency();
		m_prevTime = currentTime;
		m_currentFrameTime += dt;
		while (m_currentFrameTime > 0) {
			Update(FRAME_PERIOD);
			m_currentFrameTime -= FRAME_PERIOD;
		}
		Render();

		// try not to hammer the cpu at full throttle, still roughly 100 frames/sec
		SDL_Delay(10);
	}
}

void Game::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
		throw std::runtime_error("SDL video init failed");

	if ((m_window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN)) == NULL)
		throw std::runtime_error("Window creation failed");

	if ((m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED)) == NULL)
		throw std::runtime_error("Renderer creation failed");
	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

	AssetLoader::Init(m_renderer);

	if ((m_frameBuffer = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TextureAccess::SDL_TEXTUREACCESS_TARGET, WINDOW_W, WINDOW_H)) == NULL)
		throw std::runtime_error("Framebuffer creation failed");

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		throw std::runtime_error("SDLImage init failed");

	if (SDLNet_Init() != 0) {
		throw std::runtime_error("SDLNet_Init failed");
	}

	/*if (TTF_Init() < 0)
		throw std::runtime_error("SDL_ttf init failed");*/

	Input::Init();
	Events::Init();
	
	if (m_argc >= 2 && strcmp(m_argv[1], "-server") == 0) {
		printf("Launching in net server mode\n");
		m_sceneManager.LoadScene(std::make_unique<NetServerChessGameScene>(m_renderer));
	}
	else if (m_argc >= 2 && strcmp(m_argv[1], "-connect") == 0) {
		// TODO: NetClientChessGameScene
		printf("Net client mode is not yet implemented, exiting...\n");
		m_finished = true;
	}
	else {
		printf("Launching in local play mode\n");
		m_sceneManager.LoadScene(std::make_unique<ChessGameScene>(m_renderer));
	}
}

void Game::Shutdown()
{
	SDL_DestroyTexture(m_frameBuffer);
	m_frameBuffer = nullptr;

	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;

	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	AssetLoader::Shutdown();

	SDLNet_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::Update(float dt)
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		if (ev.type == SDL_QUIT) {
			m_finished = true;
		}
		else if (ev.type == Events::GetUserEventID(UserEventType::EVENT_GAMEOVER)) {
			// TODO: Game over screen
			//m_sceneManager.LoadScene(std::make_unique<MainMenuScene>(m_renderer));
		}
	}

	Input::Update();

	m_sceneManager.Update(dt);
}

void Game::Render()
{
	SDL_SetRenderTarget(m_renderer, m_frameBuffer);
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	SDL_RenderClear(m_renderer);

	m_sceneManager.Render();

	SDL_SetRenderTarget(m_renderer, NULL);
	SDL_RenderCopy(m_renderer, m_frameBuffer, NULL, NULL);
	SDL_RenderPresent(m_renderer);
}
