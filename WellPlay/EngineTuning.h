#pragma once

#include <string>
#include <stdint.h>
#include <float.h>
#include <functional>
#include <map>
#include <set>

class VariableGroup;
typedef void(*VarCallback)(void);

class EngineVar
{
public:
	virtual ~EngineVar() {}

	virtual void Increment( void ) {}	// DPad Right
	virtual void Decrement( void ) {}	// DPad Left
	virtual void Bang( void ) {}		// A Button
	virtual void SetCallBack(VarCallback m_callBack)
	{
		callBack = m_callBack;
	}

	//virtual void DisplayValue(CMenuElementUI* rootMenu, std::wstring& ButtonName) {}
	virtual std::string ToString( void ) const { return ""; }
	virtual void SetValue( FILE* file, const std::string& setting) = 0; //set value read from file

	EngineVar* NextVar( void );
	EngineVar* PrevVar( void );

protected:
	EngineVar();
	EngineVar( const std::string& path );
	VarCallback callBack;

private:
	friend class VariableGroup;
	VariableGroup* m_GroupPtr;
};

class BoolVar : public EngineVar
{
public:
	BoolVar( const std::string& path, bool val);
	BoolVar& operator=(bool val)
	{
		if (val != m_Flag)
		{
			m_Flag = val;
			(*callBack)();
		}
		return *this;
	}
	operator bool() const { return m_Flag; }

	virtual void Increment( void ) override 
	{
		m_Flag = !m_Flag;
		(*callBack)();
	}
	virtual void Decrement(void) override { Increment(); }
	virtual void Bang(void) override { Increment(); }

	//virtual void DisplayValue(CMenuElementUI* rootMenu, std::wstring& ButtonName) override;
	virtual std::string ToString( void ) const override;
	virtual void SetValue( FILE* file, const std::string& setting) override;

private:
	bool m_Flag;
};

class NumVar : public EngineVar
{
public:
	NumVar( const std::string& path, float val, float minValue = -FLT_MAX, float maxValue = FLT_MAX, float stepSize = 1.0f );
	NumVar& operator=(float val) { m_Value = Clamp(val); (*callBack)(); return *this; }
	operator float() const { return m_Value; }

	virtual void Increment( void ) override 
	{ 
		float temp = m_Value;
		m_Value = Clamp(m_Value + m_StepSize); 
		if (temp != m_Value)
			(*callBack)();
	}
	virtual void Decrement(void) override
	{
		float temp = m_Value;
		m_Value = Clamp(m_Value - m_StepSize);
		if (temp != m_Value)
			(*callBack)();
	}
	virtual void Bang(void) override;
	//virtual void DisplayValue(CMenuElementUI* rootMenu, std::wstring& ButtonName) override;
	virtual std::string ToString( void ) const override;
	virtual void SetValue( FILE* file, const std::string& setting)  override;

protected:
	float Clamp( float val ) { return val > m_MaxValue ? m_MaxValue : val < m_MinValue ? m_MinValue : val; }

	float m_Value;
	float m_MinValue;
	float m_MaxValue;
	float m_StepSize;
};

class ExpVar : public NumVar
{
public:
	ExpVar( const std::string& path, float val, float minExp = -FLT_MAX, float maxExp = FLT_MAX, float expStepSize = 1.0f );
	ExpVar& operator=( float val );	// m_Value = log2(val)
	operator float() const;			// returns exp2(m_Value)

	//virtual void DisplayValue(CMenuElementUI* rootMenu, std::wstring& ButtonName)  override;
	virtual std::string ToString( void ) const override;
	virtual void SetValue( FILE* file, const std::string& setting ) override;

};

class IntVar : public EngineVar
{
public:
	IntVar( const std::string& path, int32_t val, int32_t minValue = 0, int32_t maxValue = (1 << 24) - 1, int32_t stepSize = 1 );
	IntVar& operator=( int32_t val ) { m_Value = Clamp(val); (*callBack)(); return *this; }
	operator int32_t() const { return m_Value; }

	virtual void Increment( void ) override 
	{
		int temp = m_Value;
		m_Value = Clamp(m_Value + m_StepSize); 
		if(temp!=m_Value)
			(*callBack)();
	}
	virtual void Decrement( void ) override 
	{ 
		int temp = m_Value;
		m_Value = Clamp(m_Value - m_StepSize); 
		if (temp != m_Value)
			(*callBack)();
	}

	//virtual void DisplayValue(CMenuElementUI* rootMenu, std::wstring& ButtonName) override;
	virtual std::string ToString( void ) const override;
	virtual void SetValue( FILE* file, const std::string& setting ) override;

protected:
	int32_t Clamp( int32_t val ) { return val > m_MaxValue ? m_MaxValue : val < m_MinValue ? m_MinValue : val; }

	int32_t m_Value;
	int32_t m_MinValue;
	int32_t m_MaxValue;
	int32_t m_StepSize;
};

class EnumVar : public EngineVar
{
public:
	EnumVar( const std::string& path, int32_t initialVal, int32_t listLength, const char** listLabels );
	EnumVar& operator=( int32_t val ) { m_Value = Clamp(val); (*callBack)(); return *this; }
	operator int32_t() const { return m_Value; }

	virtual void Increment( void ) override 
	{ 
		m_Value = (m_Value + 1) % m_EnumLength; 
		(*callBack)();
	}
	virtual void Decrement( void ) override 
	{ 
		m_Value = (m_Value + m_EnumLength - 1) % m_EnumLength; 
		(*callBack)();
	}

	//virtual void DisplayValue(CMenuElementUI* rootMenu, std::wstring& ButtonName)  override;
	virtual std::string ToString( void ) const override;
	virtual void SetValue( FILE* file, const std::string& setting ) override;


private:
	int32_t Clamp( int32_t val ) { return val < 0 ? 0 : val >= m_EnumLength ? m_EnumLength - 1 : val; }

	int32_t m_Value;
	int32_t m_EnumLength;
	const char** m_EnumLabels;
};

class CallbackTrigger : public EngineVar
{
public:
	CallbackTrigger( const std::string& path, std::function<void (void*)> callback, void* args = nullptr );

	virtual void Bang( void ) override { m_Callback(m_Arguments); m_BangDisplay = 64; }

	//virtual void DisplayValue(CMenuElementUI* rootMenu,std::wstring& ButtonName) override;
	virtual void SetValue( FILE* file, const std::string& setting ) override;

private:
	std::function<void (void*)> m_Callback;
	void* m_Arguments;
	mutable uint32_t m_BangDisplay;
};

class GraphicsContext;

namespace EngineTuning
{
	void Initialize( void );
	void Update( float frameTime );
	//void Display(CMenuElementUI* rootMenu);
	EngineVar* Select(std::wstring& ButtonName);
	bool IsFocused( void );

} // namespace EngineTuning
