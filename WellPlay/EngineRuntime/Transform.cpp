#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"
#include "Scene.h"
#include "EngineCallBack.h"

using namespace std;

XMMATRIX Transform::GetLocalTranslationMatrix() const
{
	XMMATRIX t= XMMatrixScalingFromVector(GetLocalScale())*XMMatrixRotationQuaternion(GetLocalRotation())*XMMatrixTranslationFromVector(GetLocalPosition());
	return t;
}

XMMATRIX Transform::GetWorldTranslationMatrix() const
{
	XMMATRIX worldSRT = GetLocalTranslationMatrix();
	for (auto p = m_parent; p != nullptr; p = p->m_parent)
	{
		worldSRT *= p->GetLocalTranslationMatrix();
	}
	return worldSRT;
}

void Transform::GetWorldSRT(XMVECTOR & scale, XMVECTOR & rotation, XMVECTOR & position) const
{
	XMMATRIX worldSRT = GetWorldTranslationMatrix();
	XMMatrixDecompose(&scale, &rotation, &position, worldSRT);
}

void Transform::SetWorldSRT(FXMVECTOR scale, FXMVECTOR rotation, FXMVECTOR position)
{
	XMMATRIX parentSRT = XMMatrixIdentity();
	for (auto p = m_parent; p != nullptr; p = p->m_parent)
	{
		parentSRT *= p->GetLocalTranslationMatrix();
	}
	XMMATRIX localSRT = XMMatrixScalingFromVector(scale)*XMMatrixRotationQuaternion(rotation)*XMMatrixTranslationFromVector(position);
	localSRT = localSRT*XMMatrixInverse(nullptr, parentSRT);
	XMVECTOR s, r, t;
	XMMatrixDecompose(&s, &r, &t, localSRT);
	XMStoreFloat3(&localPosition, t);
	XMStoreFloat4(&localRotation, r);
	XMStoreFloat3(&localScale, s);
}

Transform::Transform():
	localPosition(XMFLOAT3(0,0,0)),
	localRotation(XMFLOAT4(0, 0, 0, 1)),
	localScale(XMFLOAT3(1, 1, 1)),
	m_parent(nullptr)
{
}

Transform::~Transform()
{
	m_parent.reset();
	m_children.clear();
}

void Transform::SetParent(shared_ptr<Transform> parent)
{
	if (parent == shared_from_this())
	{
		EngineCallBack::OnLog(L"不能设置自己为自己的父节点",LogMode::Error);
		return;
	}

	EngineCallBack::OnMoveGameObject(*parent->gameobject(),*gameobject(),-1);

	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	if (m_parent == nullptr)
	{
		auto s = Scene::GetCurrentScene();
		s->RemoveRootGameObject(gameobject());
	}
	else
	{
		for (auto iter = m_parent->m_children.begin(); iter != m_parent->m_children.end(); iter++)
		{
			if (*iter = shared_from_this())
			{
				m_parent->m_children.erase(iter);
				break;
			}
		}
	}

	m_parent = parent;
	if (m_parent == nullptr)
	{
		auto s = Scene::GetCurrentScene();
		s->AddRootGameObject(gameobject());
	}
	else
	{
		parent->m_children.push_back(shared_from_this());
	}

	SetWorldSRT(s, r, t);
}

void Transform::AddChild(shared_ptr<Transform> child, int index)
{
	XMVECTOR s, r, t;
	child->GetWorldSRT(s, r, t);

	EngineCallBack::OnMoveGameObject(*gameobject(), *child->gameobject(), index);

	if (child->m_parent == nullptr)
	{
		auto s = Scene::GetCurrentScene();
		s->RemoveRootGameObject(child->gameobject());
	}
	else
	{
		for (auto iter = child->m_parent->m_children.begin(); iter != child->m_parent->m_children.end(); iter++)
		{
			if (*iter = child)
			{
				child->m_parent->m_children.erase(iter);
				break;
			}
		}
	}

	child->m_parent = shared_from_this();
	if (index < 0||index>=m_children.size())
		m_children.push_back(child);
	else
	{
		auto iter = m_children.begin();
		iter += index;
		m_children.insert(iter, child);
	}

	child->SetWorldSRT(s, r, t);
}

Component* Transform::Clone()
{
	ASSERT(false, "不能克隆Transform");
	return nullptr;
}

XMVECTOR Transform::GetLocalPosition() const
{
	return XMLoadFloat3(&localPosition);
}

XMVECTOR Transform::GetLocalRotation() const
{
	return XMLoadFloat4(&localRotation);
}

XMFLOAT3 Transform::GetLocalEulerAngles() const
{
	return XMQuaternion2Euler(GetLocalRotation());
}

XMVECTOR Transform::GetLocalScale() const
{
	return XMLoadFloat3(&localScale);
}

XMVECTOR Transform::GetWorldPosition() const
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return t;
}

XMVECTOR Transform::GetWorldRotation() const
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return r;
}

XMFLOAT3 Transform::GetWorldEulerAngles() const
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternion2Euler(r);
}

XMVECTOR Transform::GetWorldScale() const
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return s;
}

void Transform::SetLocalPosition(FXMVECTOR position)
{
	XMStoreFloat3(&localPosition, position);
}

void Transform::SetLocalScale(FXMVECTOR localscale)
{
	XMStoreFloat3(&localScale, localscale);
}

void Transform::SetLocalRotation(FXMVECTOR rotation)
{
	XMStoreFloat4(&localRotation, rotation);
}

void Transform::SetWorldPosition(FXMVECTOR position)
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	SetWorldSRT(s, r, position);
}

void Transform::SetWorldScale(FXMVECTOR scale)
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	SetWorldSRT(scale, r, t);
}

void Transform::SetWorldRotation(FXMVECTOR rotation)
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	SetWorldSRT(s, rotation, t);
}

XMVECTOR Transform::GetRight() const
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(1, 0, 0, 0), r);
} 

XMVECTOR Transform::GetUp() const
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(0, 1, 0, 0), r);
}

XMVECTOR Transform::GetForward() const
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(0, 0, 1, 0), r);
}

XMVECTOR Transform::GetBehind() const
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(0, 0, -1, 0), r);
}

XMVECTOR Transform::GetLeft() const
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(-1, 0, 0, 0), r);
}

XMVECTOR Transform::GetDown() const
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(0, -1, 0, 0), r);
}

std::vector<int> Transform::GetIndexHierarchy() const
{
	std::vector<int> index;
	auto child = shared_from_this();
	while (child->m_parent != nullptr)
	{
		auto& childlist = child->m_parent->GetChildren();
		for (UINT i = 0; i < childlist.size(); i++)
		{
			if (childlist[i] == child)
			{
				index.push_back(i);
				break;
			}
		}
		child = child->m_parent;
	}
	auto s = Scene::GetCurrentScene();
	auto root = s->GetRootGameObject();
	for (int i = 0; i < root.size(); i++)
	{
		if (root[i] == child->gameobject())
		{
			index.push_back(i);
			break;
		}
	}
	assert(index.size() > 0);
	std::reverse(index.begin(), index.end());
	return index;
}