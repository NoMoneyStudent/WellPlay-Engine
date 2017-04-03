#include "stdafx.h"
#include "Animator.h"
#include "SystemTime.h"
#include "GameObject.h"
#include <limits>

using namespace std;

void Animator::Play(int index)
{
	if (index > animationClips.size() - 1)
	{
		ASSERT(false, "Ë÷Òý³¬¹ý·¶Î§");
		return;
	}
	if (currentIndex != index)
	{
		currentIndex = index;
		OnInit();
	}
	timer = 0;
	currentIndex = index;
	for (array<UINT, 3>& iter : backup)
	{
		iter = array<UINT, 3>{0, 0, 0};
	}
	m_isPlay = true;
}

void Animator::Stop()
{
	m_isPlay = false;
}

void Animator::OnInit()
{	
	if (animationClips.empty())
		return;

	backup.clear();

	for (int i = 0; i < animationClips[currentIndex]->clips.size(); i++)
	{
		Transform* tempT = gameobject()->FindRootParent()->FindChild(animationClips[currentIndex]->clips[i].first)->GetTransform();
		ASSERT(tempT != nullptr, "¹Ç÷À½ÚµãÈ±Ê§ÁË");
		aniTransform.push_back(tempT);
		backup.push_back(array<UINT, 3>{0, 0, 0});
	}
}

void Animator::Update()
{
	if (animationClips.empty())
		return;

	AnimationClip* clip = animationClips[currentIndex];
	timer += SystemTime::deltaTime();
	if (timer >clip->durning)
	{
		timer = clip->loop ? fmodf(timer, clip->durning) : clip->durning;
	}

	for (int i = 0; i < aniTransform.size(); i++)
	{
		Animation& animation = clip->clips[i].second;
		UINT Tindex = backup[i][0], Rindex = backup[i][1], Sindex = backup[i][2];

		while (animation.T[Tindex].second <= timer)
		{
			Tindex++;
		}
		ASSERT((animation.T[Tindex - 1].second <= timer) && (animation.T[Tindex].second > timer));
		double fact = (timer - animation.T[Tindex - 1].second) / (animation.T[Tindex].second - animation.T[Tindex - 1].second);
		fact = fact*fact*(3.f - 2.f*fact);
		ASSERT(fact >= 0 && fact <= 1);
		XMVECTOR t = (1 - fact)*XMLoadFloat3(&animation.T[Tindex - 1].first) + fact*XMLoadFloat3(&animation.T[Tindex].first);
		
		while (animation.S[Sindex].second <= timer)
		{
			Sindex++;
		}
		double facs = (timer - animation.S[Sindex - 1].second) / (animation.S[Sindex].second - animation.S[Sindex - 1].second);
		XMVECTOR s = (1 - facs)*XMLoadFloat3(&animation.S[Sindex - 1].first) + facs*XMLoadFloat3(&animation.S[Sindex].first);
		
		while (animation.R[Rindex].second <= timer)
		{
			Rindex++;
		}
		ASSERT((animation.R[Rindex - 1].second <= timer) && (animation.R[Rindex].second > timer));
		double facr = (timer - animation.R[Rindex - 1].second) / (animation.R[Rindex].second - animation.R[Rindex - 1].second);
		XMVECTOR r = XMQuaternionSlerp(XMLoadFloat4(&animation.R[Rindex - 1].first), XMLoadFloat4(&animation.R[Rindex].first), facr);
		r = XMVector4Normalize(r);
		ASSERT(facr >= 0 && facr <= 1);

		aniTransform[i]->SetLocalPosition(t);
		aniTransform[i]->SetLocalRotation(r);
		aniTransform[i]->SetLocalScale(s);
	}
}

Component * Animator::Clone()
{
	Animator* copy = new Animator();
	
	copy->timer = timer;
	copy->currentIndex = currentIndex;
	copy->m_isPlay = m_isPlay;
	copy->backup = backup;
	copy->animationClips = animationClips;
	return static_cast<Component*>(copy);
}

Animator::Animator()
{
}

Animator::~Animator()
{
}