#include "stdafx.h"
#include "SystemTime.h"

double SystemTime::sm_CpuTickDelta = 0.0;
int64_t SystemTime::currentTick=0;
double SystemTime::sm_deltaTime=0.0;

void SystemTime::Initialize( void )
{
	LARGE_INTEGER frequency;
	ASSERT(TRUE == QueryPerformanceFrequency(&frequency), "Unable to query performance counter frequency");
	sm_CpuTickDelta = 1.0 / static_cast<double>(frequency.QuadPart);
}

int64_t SystemTime::GetCurrentTick( void )
{
	LARGE_INTEGER currentTick;
	ASSERT(TRUE == QueryPerformanceCounter(&currentTick), "Unable to query performance counter value");
	return static_cast<int64_t>(currentTick.QuadPart);
}

void SystemTime::BusyLoopSleep( float SleepTime )
{
	int64_t finalTick = (int64_t)((double)SleepTime / sm_CpuTickDelta) + GetCurrentTick();
	while (GetCurrentTick() < finalTick);
}

void SystemTime::Update()
{
	int64_t current = GetCurrentTick();
	sm_deltaTime = TimeBetweenTicks(currentTick, current);
	currentTick = current;
}
