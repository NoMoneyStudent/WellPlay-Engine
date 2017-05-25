#pragma once
#include<memory>
#include "cereal\access.hpp"
#include <cereal\types\polymorphic.hpp>

class GameObject;
class EditorComponent;

class Component
{
	friend class GameObject;
	friend class EditorComponent;
	friend class cereal::access;
protected:
	Component() = default;
	Component(Component&) = delete;
	Component& operator=(Component&) = delete;
	virtual ~Component() { m_gameobject.reset(); }

private:
	virtual void OnInit() {}
	virtual void Update() {}
	virtual void OnDestroy() {}
	virtual void OnEnable() {}
	virtual void OnDisable() {}
	virtual Component* Clone() = 0;

	bool m_isEnable = true;
	std::shared_ptr<GameObject> m_gameobject;

#pragma region –Ú¡–ªØ
	template<class Archive>
	void save(Archive & archive) const
	{
		archive(m_isEnable, m_gameobject);
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(m_isEnable, m_gameobject);
	}
#pragma endregion
public:
	virtual void SetEnable(bool setenable);
    bool GetEnable() const { return m_isEnable; }

	std::shared_ptr<GameObject> gameobject() const { return m_gameobject; }
};