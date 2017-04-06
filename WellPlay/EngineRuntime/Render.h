#pragma once
#include "EditorComponent.h"
#include "Resource\Model.h"
#include "Render\RenderObject.h"
class GameObject;

class Render:public EditorComponent
{
	friend class GameObject;
	friend class cereal::access;

protected:
	Render();
	virtual ~Render();
	RenderCore::RenderObject render;

#pragma region ���л�
	template<class Archive>
	void save(Archive & archive) const
	{
		archive(cereal::base_class<EditorComponent>(this));
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(cereal::base_class<EditorComponent>(this));
	}
#pragma endregion
public:
};