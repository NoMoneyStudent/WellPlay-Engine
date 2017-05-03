#pragma once
struct EngineUtility
{
public:
	static bool isInPlay() { return m_isInPlay; }
	static void SetPlay(bool ifplay) { m_isInPlay = ifplay; };

private:
	static bool m_isInPlay;
};
