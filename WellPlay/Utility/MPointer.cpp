#include "stdafx.h"
#include "MPointer.h"
#include <memory>
#include <map>

std::map<void*, std::shared_ptr<bool>>& GetRList()
{
	static std::map<void*, std::shared_ptr<bool>> _list{ { nullptr, std::shared_ptr<bool>(new bool(false)) } };
	return _list;
}

ReferenceBase::ReferenceBase(void * target)
{
	findExist(target);
}

void ReferenceBase::findExist(void * target)
{
	auto& list = GetRList();

	auto index = list.find(target);
	if (index == list.end())
	{
		std::shared_ptr<bool> exist(new bool(true));
		list.insert({ target ,exist });
		ifExist = exist.get();
	}
	else
		ifExist = list[nullptr].get();
}

void PointerBase::ClearList(void* target)
{
	auto& list = GetRList();

	auto index = list.find(target);
	if (index != list.end())
	{
		list.erase(index);
	}
}
