#pragma once
class GameObject;
class Component;

class EditorComponent
{
	friend class GameObject;
protected:
	EditorComponent() {}
	EditorComponent(EditorComponent&) = delete;
	EditorComponent& operator=(EditorComponent&) = delete;
	virtual ~EditorComponent() {}

private:
	virtual void OnInit() {}
	virtual void Update() {}
	virtual void OnDestroy() {}
	virtual void OnEnable() {}
	virtual void OnDisable() {}

public:
};