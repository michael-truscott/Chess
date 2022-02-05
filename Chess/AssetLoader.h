#pragma once

#include <SDL.h>
#include <string>
#include <unordered_map>
#include <memory>

namespace AssetLoader {
	void Init(SDL_Renderer* renderer);
	void Shutdown();
	SDL_Texture* LoadTextureFile(const std::string& textureFile);
	//TTF_Font* LoadFont(const std::string& fontFile, int pointSize);
}
