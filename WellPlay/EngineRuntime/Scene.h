#pragma once
#include <memory>
class GameObject;
class SceneManager;

class Scene
{
	friend class SceneManager;
public:
	Scene();
	~Scene();
	Scene& operator=(Scene&) = delete;

	void AddRootGameObject(std::shared_ptr<GameObject> object);
	void RemoveRootGameObject(std::shared_ptr<GameObject> object);
	std::vector<std::shared_ptr<GameObject>> GetRootGameObject() { return rootObject; }

	void Update();

	static std::shared_ptr<Scene> GetCurrentScene();

private:
	std::vector<std::shared_ptr<GameObject>> rootObject;

	static std::shared_ptr<Scene> currentscene;
};