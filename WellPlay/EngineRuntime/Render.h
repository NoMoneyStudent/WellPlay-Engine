#pragma once
#include "EditorComponent.h"
#include "Component.h"
#include "Resource\Model.h"
#include "Render\RenderObject.h"

class GameObject;

class Render:public Component,public EditorComponent
{
	friend class GameObject;
protected:
	Render();
	~Render();
	RenderCore::RenderObject render;

public:

private:
	virtual Component* Clone()override;
};