#include <iostream>
#include <memory>
#include "Game.h"

int main(int argc, char** argv) {
	std::unique_ptr<Game> game;
	try
	{
		game = std::make_unique<Game>(argc, argv);
		game->Run();
	}
	catch (std::exception& ex) {
		if (SDL_WasInit(SDL_INIT_VIDEO) == SDL_INIT_VIDEO) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", ex.what(), NULL);
		}
		std::cout << "Error: " << ex.what() << std::endl;
	}
	

	return 0;
}