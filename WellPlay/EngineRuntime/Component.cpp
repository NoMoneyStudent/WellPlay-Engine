#include "stdafx.h"
#include "Component.h"

Component* Component::Clone()
{
	ASSERT(false, "������ƴ���");
	return nullptr;
}

void Component::SetEnable(bool setenable)
{
	if (setenable != m_isEnable)
	{
		m_isEnable = setenable;
		m_isEnable ? OnEnable() : OnDisable();
	}
}

bool Component::GetEnable()
{
	return m_isEnable;
}