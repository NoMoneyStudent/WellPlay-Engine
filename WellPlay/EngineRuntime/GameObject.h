#pragma once
#include <vector>
#include <utility>
#include <string>
#include "Component.h"
#include "Transform.h"

class GameObject
{
public:
	GameObject(const std::string& name = "new GameObject");
	GameObject(GameObject* prototype);
	GameObject& operator=(GameObject&) = delete;

	template<class T> T* GetComponent();
	template<class T> T* AddComponent();
	template<class T> T* GetComponentInChildren();
	template<class T> T* GetComponentInParent();
	template<class T> std::vector<T*> GetComponents();
	template<class T> std::vector<T*> GetComponentsInChildren();
	template<class T> std::vector<T*> GetComponentsInParent() ;

	GameObject* FindChild(std::string& name);
	std::vector<GameObject*> FindAllChildren(std::string& name);
	std::string GetName() { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	Transform* GetTransform() { return m_transform; }
	bool GetSelfActive() { self_active; }
	void SetActive(bool active) { self_active = active; }
	bool GetActiveInHierarchy();

	static GameObject* Find(std::string& name);
	static std::vector<GameObject*> FindAll(std::string& name);
	static void Destroy(Component* target);
	static void Destroy(GameObject* target);

private:
	std::vector<std::pair<std::string,Component*>> m_components;
	Transform* m_transform;
	std::string m_name;
	bool self_active = true;

protected:
	~GameObject();
};