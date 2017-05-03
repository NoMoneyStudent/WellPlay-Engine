#pragma once
#include "Component.h"
#include "Transform.h"
#include "Resource\Model.h"
#include "cereal\access.hpp"

class Animator:public Component
{
	friend class GameObject;
	friend class cereal::access;
public:
	Animator() = default;
	virtual ~Animator() = default;

	void Play(int index);
	void Stop();
	bool isPlay() { return m_isPlay; }

	std::vector<AnimationClip*> GetClips()const { return animationClips; };
	void SetClips(std::vector<AnimationClip*>& aniclips);

private:
	std::vector<std::weak_ptr<Transform>> aniTransform;
	std::vector<std::array<UINT, 3>> backup;
	std::vector<AnimationClip*> animationClips;

	bool m_isPlay = false;
	UINT currentIndex = 0;
	float timer = 0;

	void UpdateAvatar();

	virtual void OnInit() override;
	virtual void Update() override;
	virtual Component* Clone()override;
#pragma region –Ú¡–ªØ
	template<class Archive>
	void save(Archive & archive) const
	{
		archive(timer, currentIndex, m_isPlay);
		archive(aniTransform);
		archive(cereal::base_class<Component>(this));
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(timer, currentIndex, m_isPlay);
		archive(aniTransform);
		archive(cereal::base_class<Component>(this));
	}
#pragma endregion
};