#pragma once

#include <SDL.h>
#include "ISceneChanger.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void Update(float dt) = 0;
	virtual void Render() = 0;

	void SetSceneChanger(ISceneChanger* sceneChanger);
protected:
	ISceneChanger* m_sceneChanger;
};

