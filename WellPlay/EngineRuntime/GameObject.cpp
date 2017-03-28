#include "stdafx.h"
#include "GameObject.h"

using namespace std;

template<class T>
T * GameObject::GetComponent()
{
	for (auto& iter : m_components)
	{
		if (iter.first == typeid(T).name())
			return dynamic_cast<T*>(&iter.second);
	}
}

template<class T>
T* GameObject::AddComponent()
{
	T* newT = new T();
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

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}