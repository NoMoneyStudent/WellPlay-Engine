#pragma once
#include <string>
#include <type_traits>
#include <assert.h>

class ReferenceBase
{
protected:
	ReferenceBase(void* target);
	void findExist(void * target);

	bool* ifExist;
};

//for example, MReference<GameObject>  MReference<Transform>
template <typename DataType>
class MReference 
	:public std::enable_if<!std::is_pointer<DataType>::value, ReferenceBase>::type
{
public:
	using type = DataType;

	MReference(DataType* target = nullptr) :
		ReferenceBase(target),
		m_target(target)
	{};
	MReference(const MReference<DataType>& r):
		m_target(r.m_target),
		ifExist(r.ifExist)
	{}
	MReference& operator=(const MReference<DataType>& r)
	{
		m_target = r.m_target;
		ifExist = r.ifExist;
		return *this;
	}
	MReference& operator=(DataType* r)
	{
		m_target = r;
		findExist(m_target);
		return *this;
	}

	operator bool()
	{
		return *ifExist;
	}

	operator DataType&()
	{
		assert(*ifExist);
		return *m_target;
	}

private:
	DataType* m_target;
};

class PointerBase
{
protected:
	PointerBase() = default;
	void ClearList(void* target);
};

//for example,MPointer<GameObject> MPointer<Transform>
//这个和raw指针没什么区别，注意要小心内存泄露
template <typename DataType>
class MPointer:public PointerBase
{
public:
	MPointer(DataType* target = nullptr) :
		m_target(target) {};

	MPointer& operator=(DataType* r)
	{
		m_target = r;
		return *this;
	}
	MPointer& operator=(const MPointer<DataType>& r)
	{
		m_target = r.m_target;
		return *this;
	}

	operator DataType&()
	{
		return *m_target;
	}

	void Clear()
	{
		delete m_target;
		ClearList(m_target);
	}

private:
	typename std::enable_if<!std::is_pointer<DataType>::value, DataType*>::type m_target;
};