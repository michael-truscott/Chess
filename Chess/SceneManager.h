#pragma once

#include "ISceneChanger.h"
#include "Scene.h"
#include <memory>
#include <SDL.h>

class SceneManager : public ISceneChanger
{
public:
	SceneManager();
	void LoadScene(std::unique_ptr<Scene> scene) override;

	void Update(float dt);
	void Render();
private:
	std::unique_ptr<Scene> m_scene;
};

