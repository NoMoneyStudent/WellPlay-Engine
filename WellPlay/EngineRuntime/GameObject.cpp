#include "stdafx.h"
#include "GameObject.h"
#include "Scene.h"

using namespace std;

GameObject::GameObject(const std::string & name):
	m_name(name)
{
	InitObject();
	m_transform->OnInit();
	m_transform->OnEnable();
}

GameObject::GameObject(GameObject * prototype):
	m_name(prototype->m_name)
{
	InitObject();
	CreateHierarchy(prototype);
	InitHierarchy();
	EnableHierarchy();
}

void GameObject::CreateHierarchy(GameObject* prototype)
{
	m_transform->localPosition = prototype->m_transform->localPosition;
	m_transform->localRotation = prototype->m_transform->localRotation;
	m_transform->localScale = prototype->m_transform->localScale;
	m_transform->m_isEnable = prototype->m_transform->m_isEnable;
	m_components.reserve(prototype->m_components.size());

	for (int i = 1; i < prototype->m_components.size(); i++)
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
	for (int i = 0; i < prototype->m_transform->m_children.size(); i++)
	{
		GameObject* child = new GameObject(prototype->m_transform->m_children[i]->m_gameobject->m_name);
		child->m_transform->SetParent(m_transform);
		child->CreateHierarchy(prototype->m_transform->m_children[i]->m_gameobject);
	}
}

void GameObject::InitHierarchy()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		m_components[i].second->OnInit();
	}
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		m_transform->m_children[i]->m_gameobject->InitHierarchy();
	}
}

void GameObject::EnableHierarchy()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i].second->m_isEnable)
			m_components[i].second->OnEnable();
		else
			m_components[i].second->OnDisable();
	}
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		m_transform->m_children[i]->m_gameobject->EnableHierarchy();
	}
}

void GameObject::InitObject()
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
	m_transform = new Transform();
	m_transform->m_gameobject = this;
	m_transform->m_isEnable = true;
	m_components.push_back(make_pair(typeid(Transform).name(), static_cast<Component*>(m_transform)));
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

GameObject * GameObject::FindRootParent()
{
	Transform* temp = m_transform;
	while (temp->m_parent != nullptr)
	{
		temp = temp->m_parent;
	}
	return temp->m_gameobject;
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
	target->SetEnable(false);
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

void GameObject::DestroyHierarchy()
{
	for (auto iter = m_components.begin(); iter != m_components.end(); iter++)
	{
		iter->second->OnDestroy();
	}
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		GameObject* child = m_transform->m_children[i]->m_gameobject;
		child->DestroyHierarchy();
	}
}

void GameObject::DisableHierarchy()
{
	for (auto iter = m_components.begin(); iter != m_components.end(); iter++)
	{
		iter->second->OnDisable();
	}
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		GameObject* child = m_transform->m_children[i]->m_gameobject;
		child->DisableHierarchy();
	}
}

void GameObject::Destroy(GameObject * target)
{
	target->DisableHierarchy();
	for (int i = 0; i < target->m_transform->m_children.size(); i++)
	{
		GameObject* child = target->m_transform->m_children[i]->m_gameobject;
		GameObject::Destroy(child);
	}
	target->DestroyHierarchy();
	target->m_transform->SetParent(nullptr);
	
	for (auto iter = target->m_components.begin(); iter != target->m_components.end(); iter++)
	{
		delete iter->second;
	}
	target->m_components.clear();
	delete target;
}

bool GameObject::GetActiveInHierarchy()
{
	return self_active ? false : m_transform->m_parent->m_gameobject->GetActiveInHierarchy();
}

void GameObject::Update()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		m_components[i].second->Update();
	}
	for (int i = 0; i < m_transform->GetChildren().size(); i++)
	{
		m_transform->GetChildren()[i]->gameobject()->Update();
	}
}

void GameObject::EditorUpdate()
{
	for (int i = 0; i < m_editorcomponents.size(); i++)
	{
		m_editorcomponents[i]->Update();
	}
	for (int i = 0; i < m_transform->GetChildren().size(); i++)
	{
		m_transform->GetChildren()[i]->gameobject()->EditorUpdate();
	}
}
