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
		UpdateAvatar();
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

void Animator::SetClips(std::vector<AnimationClip*>& aniclips)
{
	animationClips = aniclips;
	UpdateAvatar();
}

void Animator::UpdateAvatar()
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
			aniTransform.push_back(weak_ptr<Transform>());
		}
		backup.push_back(array<UINT, 3>{0, 0, 0});
	}
}

void Animator::OnInit()
{	
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
		if (aniTransform[i].expired())
			continue;

		Animation& animation = clip->clips[i].second;
		UINT Tindex = backup[i][0], Rindex = backup[i][1], Sindex = backup[i][2];
		
		if (animation.T.size() > 1)
		{
			if (animation.T[Tindex].second > timer)
				Tindex = 0;
			while (animation.T[Tindex].second <= timer)
			{
				Tindex++;
				if (Tindex >= animation.T.size())
					goto Tjump;
			}
			ASSERT((animation.T[Tindex - 1].second <= timer) && (animation.T[Tindex].second > timer));
			double fact = (timer - animation.T[Tindex - 1].second) / (animation.T[Tindex].second - animation.T[Tindex - 1].second);
			fact = fact*fact*(3.f - 2.f*fact);
			ASSERT(fact >= 0 && fact <= 1);
			XMVECTOR t = (1 - fact)*XMLoadFloat3(&animation.T[Tindex - 1].first) + fact*XMLoadFloat3(&animation.T[Tindex].first);
			aniTransform[i].lock()->SetLocalPosition(t);
		}
	Tjump:
		if (animation.S.size() > 1)
		{
			if (animation.S[Sindex].second > timer)
				Sindex = 0;
			while (animation.S[Sindex].second <= timer)
			{
				Sindex++;
				if (Sindex >= animation.S.size())
					goto Sjump;
			}
			double facs = (timer - animation.S[Sindex - 1].second) / (animation.S[Sindex].second - animation.S[Sindex - 1].second);
			XMVECTOR s = (1 - facs)*XMLoadFloat3(&animation.S[Sindex - 1].first) + facs*XMLoadFloat3(&animation.S[Sindex].first);
			aniTransform[i].lock()->SetLocalScale(s);
		}
	Sjump:
		if (animation.R.size() > 1)
		{
			if (animation.R[Rindex].second > timer)
				Rindex = 0;
			while (animation.R[Rindex].second <= timer)
			{
				Rindex++;
				if (Rindex >= animation.R.size())
					goto Rjump;
			}
			ASSERT((animation.R[Rindex - 1].second <= timer) && (animation.R[Rindex].second > timer));
			double facr = (timer - animation.R[Rindex - 1].second) / (animation.R[Rindex].second - animation.R[Rindex - 1].second);
			XMVECTOR r = XMQuaternionSlerp(XMLoadFloat4(&animation.R[Rindex - 1].first), XMLoadFloat4(&animation.R[Rindex].first), facr);
			r = XMVector4Normalize(r);
			ASSERT(facr >= 0 && facr <= 1);

			aniTransform[i].lock()->SetLocalRotation(r);
		}
	Rjump: {}
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