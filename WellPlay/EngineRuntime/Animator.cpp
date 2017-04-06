#include "stdafx.h"
#include "Animator.h"
#include "SystemTime.h"
#include "GameObject.h"
#include "Wnd\LogWnd.h"
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
		auto root = gameobject()->FindRootParent();
		ASSERT(!root.expired(), "¹Ç÷À½ÚµãÈ±Ê§ÁË");
		auto tempT = root.lock()->FindChild(animationClips[currentIndex]->clips[i].first);
		if (!tempT.expired())
		{
			aniTransform.push_back(tempT.lock()->GetTransform());
		}
		else
		{
			EditorWindows::LogWnd::Print(L"¹Ç÷À½ÚµãÈ±Ê§:  " + MakeWStr(animationClips[currentIndex]->clips[i].first));
			aniTransform.push_back(weak_ptr<Transform>());
		}
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

		if (!aniTransform[i].expired())
		{
			aniTransform[i].lock()->SetLocalPosition(t);
			aniTransform[i].lock()->SetLocalRotation(r);
			aniTransform[i].lock()->SetLocalScale(s);
		}
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

Animator::Animator():Component::Component()
{
}

Animator::~Animator()
{
}