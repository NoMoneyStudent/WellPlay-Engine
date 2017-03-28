#include "stdafx.h"
#include "GameObject.h"
#include "Scene.h"

using namespace std;

template<class T>
T* GameObject::GetComponent()
{
	for (auto& iter : m_components)
	{
		if (iter.first == typeid(T).name())
			return dynamic_cast<T*>(&iter.second);
	}
	return nullptr;
}

template<class T>
T* GameObject::AddComponent()
{
	(T*) newT = new T();
	Component* newComponent = static_cast<Component>(newcomponent);
	if (newComponent!=nullptr)
	{
		m_components.push_back(make_pair(typeid(T).name(), newComponent));
		return newComponent;
	}
	else
	{
		delete newT;
		return nullptr;
	}
}

template<class T>
T* GameObject::GetComponentInChildren()
{
	(T*) mycom = GetComponent<T>();
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

template<class T>
T* GameObject::GetComponentInParent()
{
	(T*) mycom= GetComponent<T>();
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

template<class T>
std::vector<T*> GameObject::GetComponents()
{
	std::vector<T*> mycom;
	for (auto& iter : m_components)
	{
		if (iter.first == typeid(T).name())
			mycom.push_back(dynamic_cast<T*>(&iter.second));
	}
	return mycom;
}

template<class T>
std::vector<T*> GameObject::GetComponentsInChildren()
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

template<class T>
std::vector<T*> GameObject::GetComponentsInParent()
{
	std::vector<T*> result;
	std::vector<T*> mycom = GetComponents<T>();
	if (!mycom.empty())
		result.insert(result.end(), mycom.begin(), mycom.end());

	Transform* parent = m_transform->GetParent();
	if (parent != nullptr)
	{
		mycom = parent->m_gameobject->GetComponentsInParent<T>)();
		if (!mycom.empty())
			result.insert(result.end(), mycom.begin(), mycom.end());
	}
	return result;
}

GameObject::GameObject(const std::string & name)
{
	m_name = name;
	Scene* s = Scene::GetCurrentScene();
	std::vector<GameObject*> root = s->GetRootGameObject();
	int count = 0;
	for (int i = 0; i < root.size(); i++)
	{
		if (root[i]->m_name == name)
			count++;
	}
	if (count > 0)
	{
		m_name += " (" + to_string(count) + ")";
	}
	s->AddRootGameObject(this);
}

GameObject::GameObject(GameObject * prototype)
{

}

GameObject::~GameObject()
{
}

GameObject * GameObject::FindChild(std::string & name)
{
	return nullptr;
}

std::vector<GameObject*> GameObject::FindAllChildren(std::string & name)
{
	return std::vector<GameObject*>();
}

GameObject * GameObject::Find(std::string & name)
{
	return nullptr;
}

std::vector<GameObject*> GameObject::FindAll(std::string & name)
{
	return std::vector<GameObject*>();
}

void GameObject::Destroy(Component * target)
{
}

void GameObject::Destroy(GameObject * target)
{
}

bool GameObject::GetActiveInHierarchy()
{
	return false;
}
