#include "stdafx.h"
#include "Render.h"
#include "GameObject.h"

Render::Render()
{
}

Render::~Render()
{
}

Component* Render::Clone()
{
	ASSERT(false, "讲道理虚函数不会执行到这里");
	return nullptr;
}