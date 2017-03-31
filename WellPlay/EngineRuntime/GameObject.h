#pragma once
#include <vector>
#include <utility>
#include <string>
#include "Component.h"
#include "Transform.h"
class Transform;

class GameObject
{
public:
	GameObject(const std::string& name = "new GameObject");
	GameObject(GameObject* prototype);
	GameObject& operator=(GameObject&) = delete;

	void Update();

#pragma region Ä£°åº¯Êý
	template<class T> T* GetComponent()
	{
		for (auto& iter : m_components)
		{
			if (iter.first == typeid(T).name())
				return dynamic_cast<T*>(&iter.second);
		}
		return nullptr;
	}
	template<class T> T* AddComponent()
	{
		T* newT = new T();
		Component* newComponent = static_cast<Component*>(newT);
		if (newComponent != nullptr)
		{
			m_components.push_back(make_pair(typeid(T).name(), newComponent));
			newComponent->m_gameobject = this;
			newComponent->m_isEnable = true;
			return dynamic_cast<T*>(newComponent);
		}
		else
		{
			delete newT;
			return nullptr;
		}
	}
	template<class T> T* GetComponentInChildren()
	{
		T* mycom = GetComponent<T>();
		if (mycom != nullptr)
			return mycom;
		else
		{
			std::vector<Transform*>& children = m_transform->m_children;
			for (int i = 0; i < children.size(); i++)
			{
				mycom = children[i]->m_gameobject->GetComponentInChildren<T>();
				if (mycom != nullptr)
					return mycom;
			}
			return nullptr;
		}
	}
	template<class T> T* GetComponentInParent()
	{
		T* mycom = GetComponent<T>();
		if (mycom != nullptr)
			return mycom;
		else
		{
			Transform* parent = m_transform->GetParent();
			if (parent != nullptr)
				return parent->m_gameobject->GetComponentInParent<T>();
			else
				return nullptr;
		}
	}
	template<class T> std::vector<T*> GetComponents()
	{
		std::vector<T*> mycom;
		for (auto& iter : m_components)
		{
			if (iter.first == typeid(T).name())
				mycom.push_back(dynamic_cast<T*>(&iter.second));
		}
		return mycom;
	}
	template<class T> std::vector<T*> GetComponentsInChildren()
	{
		std::vector<T*> result;
		std::vector<T*> mycom = GetComponents<T>();
		if (!mycom.empty())
			result.insert(result.end(), mycom.begin(), mycom.end());

		std::vector<Transform*>& children = m_transform->m_children;
		for (int i = 0; i < children.size(); i++)
		{
			mycom = children[i]->m_gameobject->GetComponentsInChildren<T>();
			if (!mycom.empty())
				result.insert(result.end(), mycom.begin(), mycom.end());
		}

		return result;
	}
	template<class T> std::vector<T*> GetComponentsInParent()
	{
		std::vector<T*> result;
		std::vector<T*> mycom = GetComponents<T>();
		if (!mycom.empty())
			result.insert(result.end(), mycom.begin(), mycom.end());

		Transform* parent = m_transform->GetParent();
		if (parent != nullptr)
		{
			mycom = parent->m_gameobject->GetComponentsInParent<T>();
			if (!mycom.empty())
				result.insert(result.end(), mycom.begin(), mycom.end());
		}
		return result;
	}
#pragma endregion

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

	void InitName();
	void DestroyHelper();

protected:
	~GameObject();
};