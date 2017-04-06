#pragma once
struct EngineUtility
{
public:
	static bool isInPlay() { return m_isInPlay; }

private:
	static bool m_isInPlay;
};
