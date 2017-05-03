#pragma once
#include <vector>
#include <string>
#include "Component.h"
#include "EditorComponent.h"
#include "Transform.h"
#include "MeshRender.h"
#include "SkinMeshRender.h"
#include "EngineUtility.h"
#include <cereal/access.hpp>

#include "EngineCallBack.h"

class Scene;

class GameObject:public std::enable_shared_from_this<GameObject>
{
	friend class Scene;
	friend class cereal::access;
public:
	GameObject(const std::string& name="new GameObject");
	GameObject(std::shared_ptr<GameObject> prototype);
	GameObject(GameObject*) = delete;
	GameObject& operator=(GameObject&) = delete;
	~GameObject() = default;

#pragma region 模板函数
	template<class T> std::weak_ptr<T> GetComponent()
	{
		for (auto& iter : m_components)
		{
			if (iter.first == typeid(T).name())
				return std::dynamic_pointer_cast<T>(iter.second);
		}
		return std::weak_ptr<T>();
	}
	template<class T> std::weak_ptr<T> AddComponent()
	{
		shared_ptr<T> newT(new T());
		shared_ptr<Component> newComponent = std::static_pointer_cast<Component>(newT);
		ASSERT(newComponent != nullptr, "创建的类型不是component");

		m_components.push_back(make_pair(typeid(T).name(), newComponent));
		newComponent->m_gameobject = shared_from_this();
		newComponent->m_isEnable = true;

		if (EngineUtility::isInPlay())
		{
			newComponent->OnInit();
		}
		else
		{
			shared_ptr<EditorComponent> editorc = std::dynamic_pointer_cast<EditorComponent>(newT);
			if (editorc != nullptr)
			{
				m_editorcomponents.push_back(editorc);
				editorc->EditorOnEnable();
			}
		}
		return newT;
	}
	template<class T> std::weak_ptr<T> GetComponentInChildren()
	{
		std::weak_ptr<T> mycom = GetComponent<T>();
		if (!mycom .expired())
			return mycom;
		else
		{
			auto& children = m_transform->m_children;
			for (int i = 0; i < children.size(); i++)
			{
				mycom = children[i]->m_gameobject->GetComponentInChildren<T>();
				if (!mycom.expired())
					return mycom;
			}
			return std::weak_ptr<T>();
		}
	}
	template<class T> std::weak_ptr<T> GetComponentInParent()
	{
		std::weak_ptr<T> mycom = GetComponent<T>();
		if (!mycom.expired())
			return mycom;
		else
		{
			auto parent = m_transform->GetParent();
			if (parent != nullptr)
				return parent->m_gameobject->GetComponentInParent<T>();
			else
				return std::weak_ptr<T>();
		}
	}
	template<class T> std::vector<std::weak_ptr<T>> GetComponents()
	{
		std::vector<std::weak_ptr<T>> mycom;
		for (auto& iter : m_components)
		{
			if (iter.first == typeid(T).name())
				mycom.push_back(std::dynamic_pointer_cast<T>(iter.second));
		}
		return mycom;
	}
	template<class T> std::vector<std::weak_ptr<T>> GetComponentsInChildren()
	{
		std::vector<std::weak_ptr<T>> result;
		std::vector<std::weak_ptr<T>> mycom = GetComponents<T>();
		if (!mycom.empty())
			result.insert(result.end(), mycom.begin(), mycom.end());

		auto& children = m_transform->m_children;
		for (int i = 0; i < children.size(); i++)
		{
			mycom = children[i]->m_gameobject->GetComponentsInChildren<T>();
			if (!mycom.empty())
				result.insert(result.end(), mycom.begin(), mycom.end());
		}
		return result;
	}
	template<class T> std::vector<std::weak_ptr<T>> GetComponentsInParent()
	{
		std::vector<std::weak_ptr<T>> result;
		std::vector<std::weak_ptr<T>> mycom = GetComponents<T>();
		if (!mycom.empty())
			result.insert(result.end(), mycom.begin(), mycom.end());

		auto parent = m_transform->GetParent();
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
	//"不允许创建Transform组件"
	template<> std::weak_ptr<Transform> GameObject::AddComponent() = delete;
	
	//"不允许创建Render组件,创建MeshRender或SkinMeshRender"
	template<> std::weak_ptr<Render> GameObject::AddComponent() = delete;
	
	template<> std::weak_ptr<Render> GameObject::GetComponent()
	{
		auto meshrender = GetComponent<MeshRender>();
		if (meshrender.expired())
		{
			auto skinrender = GetComponent<SkinMeshRender>();
			if (skinrender.expired())
				return std::weak_ptr<Render>();
			else
				return std::static_pointer_cast<Render>(skinrender.lock());
		}
		else
			return std::static_pointer_cast<Render>(meshrender.lock());
	}
	template<> std::vector<std::weak_ptr<Render>> GameObject::GetComponents()
	{
		std::vector<std::weak_ptr<Render>> result;
		auto meshrender = GetComponents<MeshRender>();
		auto skinrender = GetComponents<SkinMeshRender>();
		result.insert(result.end(), meshrender.begin(), meshrender.end());
		result.insert(result.end(), skinrender.begin(), skinrender.end());
		return result;
	}
#pragma endregion

	std::vector<std::pair<std::string, std::shared_ptr<Component>>> GetAllComponents() const { return m_components; };

	std::weak_ptr<GameObject> FindChild(const std::string& name);
	std::vector<std::weak_ptr<GameObject>> FindAllChildren(const std::string& name);
	std::weak_ptr<GameObject> FindRootParent();
	std::string GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }

	std::weak_ptr<Transform> GetTransform() const { return m_transform; }
	bool GetSelfActive() const { return self_active; }
	void SetActive(bool active) { self_active = active; }
	bool GetActiveInHierarchy() const;

	static std::weak_ptr<GameObject> Find(const std::string& name);
	static std::vector<std::weak_ptr<GameObject>> FindAll(const std::string& name);
	
	static std::shared_ptr<GameObject> Instantiate(const std::string& name = "new GameObject");
	static std::shared_ptr<GameObject> Instantiate(std::shared_ptr<GameObject> prototype);
	
	static void Destroy(std::shared_ptr<Component>& target);
	static void Destroy(std::shared_ptr<GameObject>& target);

private:
	std::vector<std::pair<std::string, std::shared_ptr<Component>>> m_components;
	std::vector<std::shared_ptr<EditorComponent>> m_editorcomponents;
	std::shared_ptr<Transform> m_transform;
	std::string m_name;
	bool self_active = true;

	void InitObject();
	void DestroyHierarchy();
	void DisableHierarchy();
	void CreateHierarchy(std::shared_ptr<GameObject> prototype);
	void InitHierarchy();
	void EnableHierarchy();

	void Update();
	void EditorUpdate();
#pragma region 序列化
	template<class Archive>
	void save(Archive & archive) const
	{
		archive(m_name, self_active);
		archive(m_components, m_editorcomponents, m_transform);
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(m_name, self_active);
		archive(m_components, m_editorcomponents, m_transform);
		
		EngineCallBack::OnAddGameObject(*this);
	}
#pragma endregion
};