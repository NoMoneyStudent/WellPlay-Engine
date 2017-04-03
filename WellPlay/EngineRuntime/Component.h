#pragma once
class GameObject;

class Component
{
	friend class GameObject;
protected:
	Component() {}
	Component(Component&) = delete;
	Component& operator=(Component&) = delete;
    virtual ~Component() {}
	bool m_isEnable = true;

private:
	virtual void OnInit() {}
	virtual void Update() {}
	virtual void OnDestroy() {}
	virtual void OnEnable() {}
	virtual void OnDisable() {}
	virtual Component* Clone() = 0;

	GameObject* m_gameobject;
public:
	virtual void SetEnable(bool setenable);
	virtual bool GetEnable() { return m_isEnable; }

	GameObject* gameobject() { return m_gameobject; }
};