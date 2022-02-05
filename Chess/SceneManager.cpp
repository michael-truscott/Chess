#include "SceneManager.h"

ISceneChanger::~ISceneChanger()
{
}

SceneManager::SceneManager() :
	m_scene(nullptr)
{
}

void SceneManager::LoadScene(std::unique_ptr<Scene> scene)
{
	m_scene = std::move(scene);
	m_scene->SetSceneChanger(this);
}

void SceneManager::Update(float dt)
{
	m_scene->Update(dt);
}

void SceneManager::Render()
{
	m_scene->Render();
}