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
			ASSERT(false, "�����ظ����룡");
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
	ASSERT(false, "Ŀ�겻�ڳ������£�");
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