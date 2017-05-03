#include "stdafx.h"
#include "GameObject.h"
#include "Scene.h"

using namespace std;

GameObject::GameObject(const std::string & name):
	m_name(name)
{
}

GameObject::GameObject(shared_ptr<GameObject> prototype):
	m_name(prototype->m_name)
{
}

//void GameObject::CreateHierarchy(shared_ptr<GameObject> prototype)
//{
//	m_transform->localPosition = prototype->m_transform->localPosition;
//	m_transform->localRotation = prototype->m_transform->localRotation;
//	m_transform->localScale = prototype->m_transform->localScale;
//	m_transform->m_isEnable = prototype->m_transform->m_isEnable;
//	m_components.reserve(prototype->m_components.size());
//
//	for (int i = 1; i < prototype->m_components.size(); i++)
//	{
//		std::pair<std::string, shared_ptr<Component>> temp(prototype->m_components[i].first, nullptr);
//		temp.second = prototype->m_components[i].second->Clone();
//		temp.second->m_gameobject = shared_from_this();
//		temp.second->m_isEnable = prototype->m_components[i].second->m_isEnable;
//
//		m_components.push_back(temp);
//	}
//	self_active = prototype->self_active;
//	m_transform = dynamic_cast<Transform*>(m_components[0].second);
//	ASSERT(m_transform != nullptr, "游戏对象复制出现问题");
//	for (int i = 0; i < prototype->m_transform->m_children.size(); i++)
//	{
//		GameObject* child = new GameObject(prototype->m_transform->m_children[i]->m_gameobject->m_name);
//		child->m_transform->SetParent(m_transform);
//		child->CreateHierarchy(prototype->m_transform->m_children[i]->m_gameobject);
//	}
//}

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
	auto s = Scene::GetCurrentScene();
	auto root = s->GetRootGameObject();
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
	s->AddRootGameObject(shared_from_this());
	m_transform = shared_ptr<Transform>(new Transform());
	m_transform->m_gameobject = shared_from_this();
	m_transform->m_isEnable = true;
	m_components.push_back(make_pair(typeid(Transform).name(), static_pointer_cast<Component>(m_transform)));
	m_transform->OnInit();
	m_transform->OnEnable();
}

weak_ptr<GameObject> GameObject::FindChild(const std::string & name)
{
	shared_ptr<GameObject> child;
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		child = m_transform->m_children[i]->m_gameobject;
		if (name == child->m_name)
			return child;
	}
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		auto tempchild = m_transform->m_children[i]->m_gameobject->FindChild(name);
		if (!tempchild.expired())
			return tempchild;
	}
	return weak_ptr<GameObject>();
}

std::vector<weak_ptr<GameObject>> GameObject::FindAllChildren(const std::string & name)
{
	std::vector<weak_ptr<GameObject>> children;
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		auto child = m_transform->m_children[i]->m_gameobject;
		if (name == child->m_name)
			children.push_back(child);
	}
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		auto child = m_transform->m_children[i]->m_gameobject->FindAllChildren(name);
		if (!child.empty())
			children.insert(children.end(), child.begin(), child.end());
	}
	return children;
}

weak_ptr<GameObject> GameObject::FindRootParent()
{
	shared_ptr<Transform> temp = m_transform;
	while (temp->m_parent != nullptr)
	{
		temp = temp->m_parent;
	}
	return temp->m_gameobject;
}

weak_ptr<GameObject> GameObject::Find(const std::string & name)
{
	auto s = Scene::GetCurrentScene();
	auto root = s->GetRootGameObject();
	for (int i = 0; i < root.size(); i++)
	{
		if (root[i]->m_name == name)
			return root[i];
	}
	for (int i = 0; i < root.size(); i++)
	{
		weak_ptr<GameObject> child = root[i]->FindChild(name);
		if (!child.expired())
			return child;
	}
	return weak_ptr<GameObject>();
}

std::vector<weak_ptr<GameObject>> GameObject::FindAll(const std::string & name)
{
	std::vector<weak_ptr<GameObject>> result;
	auto s = Scene::GetCurrentScene();
	auto root = s->GetRootGameObject();
	for (int i = 0; i < root.size(); i++)
	{
		if (root[i]->m_name == name)
			result.push_back(root[i]);
	}
	for (int i = 0; i < root.size(); i++)
	{
		auto children = root[i]->FindAllChildren(name);
		if (!children.empty())
			result.insert(result.end(), children.begin(), children.end());
	}
	return result;
}

shared_ptr<GameObject> GameObject::Instantiate(const std::string & name)
{
	shared_ptr<GameObject> newobject(new GameObject(name));
	newobject->InitObject();

	EngineCallBack::OnAddGameObject(*newobject);

	return newobject;
}

shared_ptr<GameObject> GameObject::Instantiate(std::shared_ptr<GameObject> prototype)
{
	shared_ptr<GameObject> newobject(new GameObject(prototype));
	newobject->InitObject();
	//newobject->CreateHierarchy(prototype);
	newobject->InitHierarchy();
	newobject->EnableHierarchy();

	EngineCallBack::OnAddGameObject(*newobject);

	return newobject;
}

void GameObject::Destroy(shared_ptr<Component>& target)
{
	ASSERT(typeid(target.get()) != typeid(Transform*), "不能删除Transform组件");

	shared_ptr<GameObject> own = target->m_gameobject;
	target->SetEnable(false);
	if (EngineUtility::isInPlay())
	{
		target->OnDestroy();
	}
	else
	{
		shared_ptr<EditorComponent> ec = dynamic_pointer_cast<EditorComponent>(target);
		if (ec != nullptr)
		{
			ec->EditorOnDestroy();
			for (auto iter = own->m_editorcomponents.begin(); iter != own->m_editorcomponents.end(); iter++)
			{
				if (*iter == ec)
				{
					own->m_editorcomponents.erase(iter);
					break;
				}
			}
		}
	}
	for (auto iter = own->m_components.begin(); iter != own->m_components.end(); iter++)
	{
		if (iter->second == target)
		{
			own->m_components.erase(iter);
			break;
		}
	}
	
	//shared_ptr<Component> del(std::move(target));
	//del.~shared_ptr();
	ASSERT(target.use_count() == 1, "在其他地方存有组件的shared_ptr,组件无法被删除");
	target.reset();
}

void GameObject::DestroyHierarchy()
{
	if (EngineUtility::isInPlay())
	{
		for (auto iter = m_components.begin(); iter != m_components.end(); iter++)
		{
			iter->second->OnDestroy();
		}
	}
	else
	{
		for (auto iter = m_editorcomponents.begin(); iter != m_editorcomponents.end(); iter++)
		{
			(*iter)->EditorOnDestroy();
		}
	}

	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		shared_ptr<GameObject> child = m_transform->m_children[i]->m_gameobject;
		child->DestroyHierarchy();
	}
}

void GameObject::DisableHierarchy()
{
	if (EngineUtility::isInPlay())
	{
		for (auto iter = m_components.begin(); iter != m_components.end(); iter++)
		{
			iter->second->SetEnable(false);
		}
	}
	else
	{
		for (auto iter = m_editorcomponents.begin(); iter != m_editorcomponents.end(); iter++)
		{
			(*iter)->EditorOnDisable();
		}
	}
	for (int i = 0; i < m_transform->m_children.size(); i++)
	{
		shared_ptr<GameObject> child = m_transform->m_children[i]->m_gameobject;
		child->DisableHierarchy();
	}
}

void GameObject::Destroy(shared_ptr<GameObject>& object)
{
	shared_ptr<GameObject> target(std::move(object));
	
	EngineCallBack::OnRemoveGameObject(*target);

	target->DisableHierarchy();
	target->DestroyHierarchy();

	for (int i = 0; i < target->m_transform->m_children.size(); i++)
	{
		auto child = target->m_transform->m_children[i]->m_gameobject;
		GameObject::Destroy(child);
	}

	target->m_editorcomponents.clear();
	if (target->m_transform->m_parent == nullptr)
	{
		auto s = Scene::GetCurrentScene();
		s->RemoveRootGameObject(target);
	}
	target->m_transform.reset();
	for (auto iter = target->m_components.begin(); iter != target->m_components.end(); iter++)
	{
		//ASSERT(iter->second.use_count() == 1, "在其他地方存有组件的shared_ptr,组件无法被删除");
	}
	target->m_components.clear();
	
	ASSERT(target.use_count() == 1, "其他地方存有GameObject的shared_ptr,GameObject无法被删除");
	target.reset();
}

bool GameObject::GetActiveInHierarchy() const
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
