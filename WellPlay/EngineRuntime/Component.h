#pragma once
class GameObject;

class Component
{
	friend class GameObject;
protected:
	Component() {}
	Component& operator=(Component&) = delete;
    virtual ~Component() {}
	bool m_isEnable = true;

private:
	virtual void OnInit() {}
	virtual void Update() {}
	virtual void OnDestroy() {}
	virtual void OnEnable() {}
	virtual void OnDisable() {}
	virtual Component* Clone();

	GameObject* m_gameobject;
public:
	virtual void SetEnable(bool setenable);
	virtual bool GetEnable();

	GameObject* gameobject() { return m_gameobject; }
};