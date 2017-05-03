#pragma once
#include "EditorComponent.h"
#include "Resource\Model.h"
namespace RenderCore
{
	struct RenderObject;
}
class GameObject;

class Render:public EditorComponent
{
	friend class GameObject;
	friend class cereal::access;

protected:
	Render();
	virtual ~Render() = default;
	RenderCore::RenderObject* render;

#pragma region –Ú¡–ªØ
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