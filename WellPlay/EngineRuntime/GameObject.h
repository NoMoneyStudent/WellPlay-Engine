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
	//创建GameObject应该使用GameObject::Instantiate
	GameObject(const std::string& name="new GameObject");
	GameObject(std::shared_ptr<GameObject> prototype);
	GameObject(GameObject*) = delete;
	GameObject& operator=(GameObject&) = delete;
	~GameObject() = default;

#pragma region 模板函数
private:
	template<typename comType,typename returnType> struct ComTypeCheck
		:std::enable_if<std::is_base_of<Component, comType>::value, returnType>{};
	template<typename comType> struct ComSingle
		:ComTypeCheck<comType,std::weak_ptr<comType>> {};
	template<typename comType> struct ComVector
		:ComTypeCheck<comType, std::vector<std::weak_ptr<comType>>> {};
	template<typename comType> struct EditorComTypeCheck
		:std::enable_if<std::is_base_of<EditorComponent, comType>::value, void> {};

	template<typename T> 
	inline auto AddEditorComponent(std::shared_ptr<T> target)
		->typename EditorComTypeCheck<T>::type
	{
		shared_ptr<EditorComponent> editorc = std::static_pointer_cast<EditorComponent>(target);
		assert(editorc != nullptr);
		m_editorcomponents.push_back(editorc);
		editorc->EditorOnInit();
		editorc->EditorOnEnable();
	}
	template<typename T>
	inline auto AddEditorComponent(std::shared_ptr<T> target)
		->typename std::enable_if<!std::is_base_of<EditorComponent, T>::value, void>::type {}

public:
	template<class T> auto GetComponent()
		->typename ComSingle<T>::type
	{
		for (auto& iter : m_components)
		{
			if (typeid(*iter) == typeid(T))
			{
				auto res = std::dynamic_pointer_cast<T>(iter);
				assert(res != nullptr);
				return res;
			}
		}
		return std::weak_ptr<T>();
	}
	template<class T> auto GetComponents()
		->typename ComVector<T>::type
	{
		std::vector<std::weak_ptr<T>> mycom;
		for (auto& iter : m_components)
		{
			if (typeid(*iter) == typeid(T))
			{
				auto res = std::dynamic_pointer_cast<T>(iter);
				assert(res != nullptr);
				mycom.push_back(res);
			}
		}
		return mycom;
	}

	template<class T> auto AddComponent()
		->typename ComSingle<T>::type
	{
		std::shared_ptr<T> newT(new T());
		std::shared_ptr<Component> newComponent = std::static_pointer_cast<Component>(newT);
		assert(newComponent != nullptr);

		m_components.push_back(newComponent);
		newComponent->m_gameobject = shared_from_this();
		newComponent->m_isEnable = true;

		if (EngineUtility::isInPlay())
		{
			newComponent->OnInit();
			newComponent->OnEnable();
		}
		else
		{
			AddEditorComponent(newT);
		}
		return newT;
	}
	template<class T> auto GetComponentInChildren()
		->typename ComSingle<T>::type
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
	template<class T> auto GetComponentInParent()
		->typename ComSingle<T>::type
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
	template<class T> auto GetComponentsInChildren()
		->typename ComVector<T>::type
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
	template<class T> auto GetComponentsInParent()
		->typename ComVector<T>::type
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
private:
	//"不允许GameObject外部创建Transform组件"
	template<> std::weak_ptr<Transform> AddComponent<Transform>()
	{
		std::shared_ptr<Transform> newT(new Transform());
		std::shared_ptr<Component> newComponent = std::static_pointer_cast<Component>(newT);
		assert(newComponent != nullptr);

		newComponent->m_gameobject = shared_from_this();
		newComponent->m_isEnable = true;
		m_transform = newT;
		
		return newT;
	}
public:
	//"不允许创建Render组件,创建MeshRender或SkinMeshRender"
	template<> std::weak_ptr<Render> AddComponent<Render>() = delete;
	//最好使用GetTransform()
	template<> std::weak_ptr<Transform> GetComponent<Transform>()
	{
		return m_transform;
	}
	//最好使用GetTransform()
	template<> std::vector<std::weak_ptr<Transform>> GetComponents<Transform>()
	{
		std::vector<std::weak_ptr<Transform>> res(1);
		res[0] = m_transform;
		return res;
	}
	template<> std::weak_ptr<Render> GetComponent<Render>()
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
	template<> std::vector<std::weak_ptr<Render>> GetComponents<Render>()
	{
		std::vector<std::weak_ptr<Render>> result;
		auto meshrender = GetComponents<MeshRender>();
		auto skinrender = GetComponents<SkinMeshRender>();
		result.insert(result.end(), meshrender.begin(), meshrender.end());
		result.insert(result.end(), skinrender.begin(), skinrender.end());
		return result;
	}
#pragma endregion

	const auto GetAllComponents() const { return m_components; };

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
	std::vector<std::shared_ptr<Component>> m_components;
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
		archive(m_transform);
		archive(m_components, m_editorcomponents);
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(m_name, self_active);
		archive(m_transform);
		archive(m_components, m_editorcomponents);
		
		EngineCallBack::OnAddGameObject(*this);
	}
#pragma endregion
};