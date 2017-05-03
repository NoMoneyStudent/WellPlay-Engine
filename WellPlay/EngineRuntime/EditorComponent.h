#pragma once
#include"Component.h"

class GameObject;

class EditorComponent:public Component
{
	friend class GameObject;
	friend class cereal::access;

protected:
	EditorComponent() = default;
	EditorComponent(EditorComponent&) = delete;
	EditorComponent& operator=(EditorComponent&) = delete;
	virtual ~EditorComponent() = default;

private:
	virtual void EditorOnInit() {}
	virtual void EditorUpdate() {}
	virtual void EditorOnDestroy() {}
	virtual void EditorOnEnable() {}
	virtual void EditorOnDisable() {}
#pragma region –Ú¡–ªØ
	template<class Archive>
	void save(Archive & archive) const
	{
		archive(cereal::base_class<Component>(this));
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(cereal::base_class<Component>(this));
	}
#pragma endregion

public:
	virtual void SetEnable(bool setenable)override;
};