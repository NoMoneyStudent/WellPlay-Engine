#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"
#include "Scene.h"

using namespace std;

XMMATRIX Transform::GetLocalTranslationMatrix()
{
	XMMATRIX t= XMMatrixScalingFromVector(GetLocalScale())*XMMatrixRotationQuaternion(GetLocalRotation())*XMMatrixTranslationFromVector(GetLocalPosition());
	return t;
}

XMMATRIX Transform::GetWorldTranslationMatrix()
{
	XMMATRIX worldSRT = GetLocalTranslationMatrix();
	for (auto p = m_parent; p != nullptr; p = p->m_parent)
	{
		worldSRT *= p->GetLocalTranslationMatrix();
	}
	return worldSRT;
}

void Transform::GetWorldSRT(XMVECTOR & scale, XMVECTOR & rotation, XMVECTOR & position)
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
	Component::Component(),
	enable_shared_from_this<Transform>::enable_shared_from_this(),
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
	Component::~Component();
}

void Transform::SetParent(shared_ptr<Transform> parent)
{
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
	ASSERT(false, "²»ÄÜ¿ËÂ¡Transform");
	return nullptr;
}

XMVECTOR Transform::GetLocalPosition()
{
	return XMLoadFloat3(&localPosition);
}

XMVECTOR Transform::GetLocalRotation()
{
	return XMLoadFloat4(&localRotation);
}

XMFLOAT3 Transform::GetLocalEulerAngles()
{
	return XMQuaternion2Euler(GetLocalRotation());
}

XMVECTOR Transform::GetLocalScale()
{
	return XMLoadFloat3(&localScale);
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

XMVECTOR Transform::GetRight()
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(1, 0, 0, 0), r);
}

XMVECTOR Transform::GetUp()
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(0, 1, 0, 0), r);
}

XMVECTOR Transform::GetForward()
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(0, 0, 1, 0), r);
}

XMVECTOR Transform::GetBehind()
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(0, 0, -1, 0), r);
}

XMVECTOR Transform::GetLeft()
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(-1, 0, 0, 0), r);
}

XMVECTOR Transform::GetDown()
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	return XMQuaternionRotate(XMVectorSet(0, -1, 0, 0), r);
}