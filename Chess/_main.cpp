#include <SDL.h>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return -1;
	}
	SDL_Window* window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Surface* surface = SDL_GetWindowSurface(window);

	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
	SDL_UpdateWindowSurface(window);
	SDL_Delay(2000);

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}