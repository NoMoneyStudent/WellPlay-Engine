#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"

Scene* Scene::currentscene = nullptr;

void Scene::AddRootGameObject(GameObject * object)
{
#ifdef _DEBUG
	for (auto iter = rootObject.begin(); iter != rootObject.end(); iter++)
	{
		if (*iter == object)
		{
			ASSERT(false, "不能重复加入！");
			return;
		}
	}
#endif // _DEBUG

	rootObject.push_back(object);
}

void Scene::RemoveRootGameObject(GameObject * object)
{
	for (auto iter = rootObject.begin(); iter != rootObject.end();iter++)
	{
		if (*iter == object)
		{
			rootObject.erase(iter);
			return;
		}
	}
	ASSERT(false, "目标不在场景根下！");
}

void Scene::Update()
{
	for (int i = 0; i < rootObject.size(); i++)
	{
		rootObject[i]->Update();
	}
}

Scene * Scene::GetCurrentScene()
{
	if (currentscene == nullptr)
	{
		currentscene = new Scene();
	}
	return currentscene;
}

Scene::Scene()
{
}

Scene::~Scene()
{
}