#include "stdafx.h"
#include "Scene.h"

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

Scene::Scene()
{
}

Scene::~Scene()
{
}