#include "AssetLoader.h"
#include <stdexcept>
#include <SDL_image.h>

namespace AssetLoader {

	namespace {
		std::unordered_map<std::string, SDL_Texture*> m_textureFileMap;
		//std::unordered_map<std::string, TTF_Font*> m_fontFileMap;
		SDL_Renderer* m_renderer;
	}

	void Init(SDL_Renderer* renderer) {
		m_renderer = renderer;
	}

	void Shutdown() {
		for (auto& entry : m_textureFileMap) {
			SDL_DestroyTexture(entry.second);
		}
		m_textureFileMap.clear();

		/*for (auto& entry : m_fontFileMap) {
			TTF_CloseFont(entry.second);
		}
		m_fontFileMap.clear();*/
	}

	SDL_Texture* LoadTextureFile(const std::string& textureFile) {
		auto it = m_textureFileMap.find(textureFile);
		if (it != m_textureFileMap.end())
			return it->second;

		SDL_Texture* result = IMG_LoadTexture(m_renderer, textureFile.c_str());
		if (result == nullptr)
			throw std::invalid_argument(IMG_GetError());

		m_textureFileMap.emplace(textureFile, result);
		return result;
	}
	
	/*TTF_Font* LoadFont(const std::string& fontFile, int pointSize)
	{
		std::string key = fontFile + std::to_string(pointSize);
		auto it = m_fontFileMap.find(key);
		if (it != m_fontFileMap.end())
			return it->second;

		TTF_Font* result = TTF_OpenFont(fontFile.c_str(), pointSize);
		if (result == nullptr)
			throw std::invalid_argument(IMG_GetError());

		m_fontFileMap.emplace(key, result);
		return result;
	}*/
}
