#pragma once
#include <vector>
#include <utility>
#include <string>
#include "Component.h"
#include "EditorComponent.h"
#include "Transform.h"

#include "MeshRender.h"
#include "SkinMeshRender.h"

class Scene;

class GameObject
{
	friend class Scene;
public:
	GameObject(const std::string& name = "new GameObject");
	GameObject(GameObject* prototype);
	GameObject& operator=(GameObject&) = delete;

#pragma region 模板函数
	template<class T> T* GetComponent()
	{
		for (auto& iter : m_components)
		{
			if (iter.first == typeid(T).name())
				return dynamic_cast<T*>(iter.second);
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
			EditorComponent* editorc = dynamic_cast<EditorComponent*>(newT);
			if (editorc != nullptr)
				AddEditorComponent(editorc);
			newComponent->OnInit();
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
				mycom.push_back(dynamic_cast<T*>(iter.second));
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

#pragma region 模板特化
	template<>
	Transform* GameObject::AddComponent()
	{
		ASSERT(false, "不允许创建Transform组件");
		return nullptr;
	}
	template<>
	Render* GameObject::AddComponent()
	{
		ASSERT(false, "不允许创建Render组件,替换成MeshRender或SkinMeshRender");
		return nullptr;
	}
	template<>
	Render* GameObject::GetComponent()
	{
		MeshRender* meshrender = GetComponent<MeshRender>();
		if (meshrender == nullptr)
		{
			SkinMeshRender* skinrender = GetComponent<SkinMeshRender>();
			return static_cast<Render*>(skinrender);
		}
		else
			return static_cast<Render*>(meshrender);
	}
	template<>
	std::vector<Render*> GameObject::GetComponents()
	{
		std::vector<Render*> result;
		std::vector<MeshRender*> meshrender = GetComponents<MeshRender>();
		std::vector<SkinMeshRender*> skinrender = GetComponents<SkinMeshRender>();
		result.insert(result.end(), meshrender.begin(), meshrender.end());
		result.insert(result.end(), skinrender.begin(), skinrender.end());
		return result;
	}
#pragma endregion
	/*此函数在创建组件时自动调用，无需重复调用*/
	void AddEditorComponent(EditorComponent* component)
	{
		m_editorcomponents.push_back(component);
	}
	/*在删除编辑器组件时必须调用，否则会产生空指针*/
	void RemoveEditorComponent(EditorComponent* component)
	{
		for (auto iter = m_editorcomponents.begin(); iter != m_editorcomponents.end(); iter++)
		{
			if (*iter == component)
			{
				m_editorcomponents.erase(iter);
				break;
			}
		}
	}

	GameObject* FindChild(std::string& name);
	std::vector<GameObject*> FindAllChildren(std::string& name);
	GameObject* FindRootParent();
	std::string GetName() { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	Transform* GetTransform() { return m_transform; }
	bool GetSelfActive() { self_active; }
	void SetActive(bool active) { self_active = active; }
	bool GetActiveInHierarchy();

	static GameObject* Find(std::string& name);
	static std::vector<GameObject*> FindAll(std::string& name);
	
	/*在删除编辑器组件时必须调用RemoveEditorComponent*/
	static void Destroy(Component* target);
	static void Destroy(GameObject* target);

private:
	std::vector<std::pair<std::string,Component*>> m_components;
	std::vector<EditorComponent*> m_editorcomponents;
	Transform* m_transform;
	std::string m_name;
	bool self_active = true;

	void InitObject();
	void DestroyHierarchy();
	void DisableHierarchy();
	void CreateHierarchy(GameObject* prototype);
	void InitHierarchy();
	void EnableHierarchy();
	void Update();
	void EditorUpdate();

	~GameObject();
};