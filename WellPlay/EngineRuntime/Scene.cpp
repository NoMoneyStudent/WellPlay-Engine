#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"
#include "EngineUtility.h"

std::shared_ptr<Scene> Scene::currentscene;

void Scene::AddRootGameObject(std::shared_ptr<GameObject> object)
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

void Scene::RemoveRootGameObject(std::shared_ptr<GameObject> object)
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
		EngineUtility::isInPlay() ? rootObject[i]->Update() : rootObject[i]->EditorUpdate();
	}
}

std::shared_ptr<Scene> Scene::GetCurrentScene()
{
	if (currentscene == nullptr)
	{
		currentscene = std::shared_ptr<Scene>(new Scene());
		ASSERT(currentscene != nullptr, "创建场景失败了");
	}
	return currentscene;
}

Scene::Scene()
{
}

Scene::~Scene()
{
}