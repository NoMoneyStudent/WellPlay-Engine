#pragma once

class SystemTime
{
public:
	static void Initialize( void );

	static int64_t GetCurrentTick( void );

	static void BusyLoopSleep( float SleepTime );

	static void Update();
	static double deltaTime() { return sm_deltaTime; }const

	static inline double TicksToSeconds( int64_t TickCount )
	{
		return TickCount * sm_CpuTickDelta;
	}

	static inline double TicksToMillisecs( int64_t TickCount )
	{
		return TickCount * sm_CpuTickDelta * 1000.0;
	}

	static inline double TimeBetweenTicks( int64_t tick1, int64_t tick2 )
	{
		return TicksToSeconds(tick2 - tick1);
	}

private:

	static double sm_CpuTickDelta;
	static int64_t currentTick;
	static double sm_deltaTime;
};

class CpuTimer
{
public:

	CpuTimer()
	{
		m_StartTick = 0ll;
		m_ElapsedTicks = 0ll;
	}

	void Start()
	{
		if (m_StartTick == 0ll)
			m_StartTick = SystemTime::GetCurrentTick();
	}

	void Stop()
	{
		if (m_StartTick != 0ll)
		{
			m_ElapsedTicks += SystemTime::GetCurrentTick() - m_StartTick;
			m_StartTick = 0ll;
		}
	}

	void Reset()
	{
		m_ElapsedTicks = 0ll;
		m_StartTick = 0ll;
	}

	double GetTime() const
	{
		return SystemTime::TicksToSeconds(m_ElapsedTicks);
	}

private:

	int64_t m_StartTick;
	int64_t m_ElapsedTicks;
};
