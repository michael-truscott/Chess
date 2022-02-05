#include "Scene.h"

Scene::Scene() :
	m_sceneChanger(nullptr)
{
}

Scene::~Scene()
{
}

void Scene::SetSceneChanger(ISceneChanger* sceneChanger)
{
	m_sceneChanger = sceneChanger;
}
