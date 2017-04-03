#pragma once
#include "Component.h"
#include "Transform.h"
#include "Resource\Model.h"

class Animator:public Component
{
	friend class GameObject;
public:
	std::vector<AnimationClip*> animationClips;
	void Play(int index);
	void Stop();
	bool isPlay() { return m_isPlay; }

private:
	std::vector<Transform*> aniTransform;
	std::vector<std::array<UINT, 3>> backup;

	bool m_isPlay = false;
	UINT currentIndex = 0;
	float timer = 0;

	virtual void OnInit() override;
	virtual void Update() override;
	virtual Component* Clone()override;

	Animator();
	~Animator();
};