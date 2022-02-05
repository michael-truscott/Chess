#include <iostream>
#include "Game.h"

int main(int argc, char** argv) {
	try
	{
		Game game;
		game.Run();
	}
	catch (std::exception& ex) {
		std::cout << "Error: " << ex.what() << std::endl;
	}

	return 0;
}