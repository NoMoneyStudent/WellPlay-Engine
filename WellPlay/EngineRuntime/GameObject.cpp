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
		newComponent->m_gameobject = this;
		newComponent->m_isEnable = true;
		return newComponent;
	}
	else
	{
		delete newT;
		return nullptr;
	}
}

template<>
Transform* GameObject::AddComponent()
{
	ASSERT(false, "不允许创建Transform组件");
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

GameObject::GameObject(const std::string & name):
	m_name(name)
{
	InitName();
}

GameObject::GameObject(GameObject * prototype):
	m_name(prototype->m_name)
{
	InitName();

	m_components.reserve(prototype->m_components.size());
	for (int i = 0; i < prototype->m_components.size(); i++)
	{
		std::pair<std::string, Component*> temp(prototype->m_components[i].first, nullptr);
		temp.second = prototype->m_components[i].second->Clone();
		temp.second->m_gameobject = this;
		temp.second->m_isEnable = prototype->m_components[i].second->m_isEnable;

		m_components.push_back(temp);
	}
	self_active = prototype->self_active;
	m_transform = dynamic_cast<Transform*>(m_components[0].second);
	ASSERT(m_transform != nullptr, "游戏对象复制出现问题");
}

void GameObject::InitName()
{
	Scene* s = Scene::GetCurrentScene();
	std::vector<GameObject*> root = s->GetRootGameObject();
	int count = 0;
	for (int i = 0; i < root.size(); i++)
	{
		if (root[i]->m_name == m_name)
			count++;
	}
	if (count > 0)
	{
		m_name += " (" + to_string(count) + ")";
	}
	s->AddRootGameObject(this);
}

GameObject::~GameObject()
{
}

GameObject * GameObject::FindChild(std::string & name)
{
	GameObject* child;
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		child = m_transform->m_children[i]->m_gameobject;
		if (name == child->m_name)
			return child;
	}
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		child = m_transform->m_children[i]->m_gameobject->FindChild(name);
		if (child!=nullptr)
			return child;
	}
	return nullptr;
}

std::vector<GameObject*> GameObject::FindAllChildren(std::string & name)
{
	std::vector<GameObject*> children;
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		GameObject* child = m_transform->m_children[i]->m_gameobject;
		if (name == child->m_name)
			children.push_back(child);
	}
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		std::vector<GameObject*> child = m_transform->m_children[i]->m_gameobject->FindAllChildren(name);
		if (!child.empty())
			children.insert(children.end(), child.begin(), child.end());
	}
	return children;
}

GameObject * GameObject::Find(std::string & name)
{
	Scene* s = Scene::GetCurrentScene();
	std::vector<GameObject*> root = s->GetRootGameObject();
	for (int i = 0; i < root.size(); i++)
	{
		if (root[i]->m_name == name)
			return root[i];
	}
	for (int i = 0; i < root.size(); i++)
	{
		GameObject* child = root[i]->FindChild(name);
		if (child != nullptr)
			return child;
	}
	return nullptr;
}

std::vector<GameObject*> GameObject::FindAll(std::string & name)
{
	std::vector<GameObject*> result;
	Scene* s = Scene::GetCurrentScene();
	std::vector<GameObject*> root = s->GetRootGameObject();
	for (int i = 0; i < root.size(); i++)
	{
		if (root[i]->m_name == name)
			result.push_back(root[i]);
	}
	for (int i = 0; i < root.size(); i++)
	{
		std::vector<GameObject*> children = root[i]->FindAllChildren(name);
		if (!children.empty())
			result.insert(result.end(), children.begin(), children.end());
	}
	return result;
}

void GameObject::Destroy(Component * target)
{
	ASSERT(typeid(target) != typeid(Transform), "不能删除Transform组件");
	GameObject* own = target->m_gameobject;
	target->OnDestroy();
	for (auto iter = own->m_components.begin(); iter != own->m_components.end(); iter++)
	{
		if (iter->second == target)
		{
			own->m_components.erase(iter);
			break;
		}
	}
	
	delete target;
}

void GameObject::DestroyHelper()
{
	for (auto iter = m_components.begin(); iter != m_components.end(); iter++)
	{
		iter->second->OnDestroy();
	}
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		GameObject* child = m_transform->m_children[i]->m_gameobject;
		child->DestroyHelper();
	}
}

void GameObject::Destroy(GameObject * target)
{
	target->DestroyHelper();
	for (int i = 0; i < target->m_transform->m_children.size(); i++)
	{
		GameObject* child = target->m_transform->m_children[i]->m_gameobject;
		GameObject::Destroy(child);
	}
	target->m_transform->SetParent(nullptr);
	
	for (auto iter = target->m_components.begin(); iter != target->m_components.end(); iter++)
	{
		delete iter->second;
	}

	delete target;
}

bool GameObject::GetActiveInHierarchy()
{
	return self_active ? false : m_transform->m_parent->m_gameobject->GetActiveInHierarchy();
}