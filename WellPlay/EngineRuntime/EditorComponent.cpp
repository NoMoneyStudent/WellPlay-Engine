#include "stdafx.h"
#include "EditorComponent.h"
#include"EngineUtility.h"

void EditorComponent::SetEnable(bool setenable)
{
	if (EngineUtility::isInPlay())
	{
		Component::SetEnable(setenable);
	}
	else
	{
		if (setenable != m_isEnable)
		{
			m_isEnable = setenable;
			m_isEnable ? EditorOnEnable() : EditorOnDisable();
		}
	}
}