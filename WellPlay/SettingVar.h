#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <assert.h>
#include <boost\noncopyable.hpp>
#include "rttr\rttr_enable.h"

using ChangedCallBack = std::function<void(void)>;
class SettingBase;

const std::vector<SettingBase*>& GetSettingList();

class SettingBase :public boost::noncopyable
{
	RTTR_ENABLE()
protected:
	SettingBase(const std::wstring& path, const ChangedCallBack& callback);
	virtual ~SettingBase() = default;
public:
	std::wstring GetPath() const { return m_path; };
	void RegisterOnChanged(const ChangedCallBack& callback)
	{
		m_OnChanged = callback;
	}
	virtual std::wstring toString() = 0;

protected:
	void OnChanged()
	{
		if (m_OnChanged)
			m_OnChanged();
	};

private:
	std::wstring m_path;
	ChangedCallBack m_OnChanged;
};

template <typename DataType, typename Enable = void>
class SettingVar :public SettingBase
{
private:
	SettingVar();
};

// 整数和浮点数类型
template <typename DataType>
class SettingVar<DataType,typename std::enable_if<std::is_arithmetic<DataType>::value>::type>
	:public SettingBase
{
	RTTR_ENABLE(SettingBase)
public:
	SettingVar(const std::wstring& path, DataType data, DataType min, DataType max, const ChangedCallBack& callback = ChangedCallBack()) :
		m_data(data),
		m_min(min),
		m_max(max),
		SettingBase(path, callback)
	{
		assert(m_data >= m_min);
		assert(m_data <= m_max);
	}
	~SettingVar() = default;

	auto& operator=(const DataType& val)
	{
		if (m_data != val)
		{
			m_data = Clamp(val);
			OnChanged();
		}
		return *this;
	}
	operator DataType() const { return m_data; }

	virtual std::wstring toString()
	{
		return std::to_wstring(m_data);
	}

	DataType GetMin()const { return m_min; };
	DataType GetMax()const { return m_max; };
    
private:
	DataType m_min;
	DataType m_max;
	DataType m_data;

	DataType Clamp(DataType val) { return val > m_max ? m_max : val < m_min ? m_min : val; }
};

//用来标志枚举
struct EnumType 
{
	using type = unsigned int;
};

//这个是内部用的
template <> class SettingVar<EnumType, void> :public SettingBase
{
	RTTR_ENABLE(SettingBase)

	using type = typename EnumType::type;
public:
	SettingVar(const std::wstring& path, type data, std::vector<std::wstring>& list, ChangedCallBack& callback = ChangedCallBack()) :
		m_data(data),
		m_list(std::move(list)),
		SettingBase(path, callback)
	{}
	~SettingVar() = default;

	void SetData(type val)
	{
		if (m_data != val)
		{
			m_data = val;
			OnChanged();
		}
	}
	type GetData() const { return m_data; }
	const auto& GetList() const { return m_list; }

	virtual std::wstring toString()
	{
		return m_list[static_cast<int>(m_data)];
	}

protected:
	type m_data;
	std::vector<std::wstring> m_list;
};

//枚举类型 必须是从0开始，一个一个过去
template <typename DataType>
class SettingVar<DataType, typename std::enable_if<std::is_enum<DataType>::value>::type>
	:public SettingVar<EnumType,void>
{
public:
	SettingVar(const std::wstring& path, DataType data, std::vector<std::wstring>& list, ChangedCallBack& callback = ChangedCallBack()) :
		SettingVar<EnumType, void>(path, static_cast<EnumType::type>(data), list, callback)
	{};
	~SettingVar() = default;

	auto& operator=(DataType val)
	{
		SetData(static_cast<EnumType::type>(val));
		return *this;
	}
	operator DataType() const { return static_cast<EnumType::type>(GetData()); };
};
#include "Utility\MPointer.h"

//MReference类型 for example,SettingVar<MReference<GameObject>>
template <typename DataType>
class SettingVar<DataType, typename std::enable_if<std::is_base_of<ReferenceBase,DataType>::value>::type>
	:public SettingBase
{
	RTTR_ENABLE(SettingBase)
public:
	SettingVar(const std::wstring& path, DataType data, ChangedCallBack& callback = ChangedCallBack()) :
		m_data(data),
		SettingBase(path, callback)
	{}
	~SettingVar() = default;

	auto& operator=(std::weak_ptr<DataType> val)
	{
		if (m_data != val)
		{
			m_data = val;
			OnChanged();
		}
		return *this;
	}
	operator bool() const
	{
		return !m_data.expired();
	}

	using type = typename DataType::type;

	operator type&() const
	{
		assert(!m_data);
		return m_data;
	}

	virtual std::wstring toString()
	{
		return GetName<DataType>();
	}

private:
	DataType m_data;

	class Component;
	template<typename T>
	std::wstring GetName()
	{
		static_assert(std::is_base_of<Component, T>::value);
		return m_data.gameobject().GetName();
	}

	class GameObject;
	template<> std::wstring GetName<GameObject>()
	{
		return m_data.GetName();
	}
};

//宽字符串
template <>
class SettingVar<std::wstring,void> :public SettingBase
{
	RTTR_ENABLE(SettingBase)
public:
	SettingVar(const std::wstring& path, const std::wstring data, ChangedCallBack& callback = ChangedCallBack()) :
		m_data(data),
		SettingBase(path, callback)
	{}
	~SettingVar() = default;

	auto& operator=(std::wstring& val)
	{
		m_data = val;
		OnChanged();
		return *this;
	}
	operator const std::wstring&() const { return m_data; }

	virtual std::wstring toString()
	{
		return m_data;
	}

private:
	std::wstring m_data;
};

//布尔类型
template <>
class SettingVar<bool,void> :public SettingBase
{
	RTTR_ENABLE(SettingBase)
public:
	SettingVar(const std::wstring& path, bool data, ChangedCallBack& callback = ChangedCallBack()) :
		m_data(data),
		SettingBase(path, callback)
	{}
	~SettingVar() = default;

	auto& operator=(bool val)
	{
		if (m_data != val)
		{
			m_data = val;
			OnChanged();
		}
		return *this;
	}
	operator bool() const { return m_data; }
	virtual std::wstring toString()
	{
		return m_data ? L"True" : L"False";
	}

private:
	bool m_data;
};