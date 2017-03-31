#pragma once
class GameObject;
class SceneManager;

class Scene
{
	friend class SceneManager;
public:
	Scene& operator=(Scene&) = delete;

	void AddRootGameObject(GameObject* object);
	void RemoveRootGameObject(GameObject* object);
	std::vector<GameObject*> GetRootGameObject() { return rootObject; }

	void Update();

	static Scene* GetCurrentScene();

private:
	Scene();
	~Scene();
	std::vector<GameObject*> rootObject;

	static Scene* currentscene;
};