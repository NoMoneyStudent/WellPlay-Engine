#include "stdafx.h"
#include "Component.h"

void Component::SetEnable(bool setenable)
{
	if (setenable != m_isEnable)
	{
		m_isEnable = setenable;
		m_isEnable ? OnEnable() : OnDisable();
	}
}