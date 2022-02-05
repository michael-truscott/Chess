#pragma once

#include <memory>

class Scene;

class ISceneChanger {
public:
	virtual ~ISceneChanger();
	virtual void LoadScene(std::unique_ptr<Scene> scene) = 0;
};