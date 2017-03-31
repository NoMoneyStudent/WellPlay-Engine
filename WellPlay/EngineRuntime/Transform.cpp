#include "stdafx.h"
#include "Transform.h"
#include "Scene.h"

XMMATRIX Transform::GetLocalTranslationMatrix()
{
	XMMATRIX t= XMMatrixScalingFromVector(GetLocalScale())*XMMatrixRotationQuaternion(GetLocalRotation())*XMMatrixTranslationFromVector(GetLocalPosition());
	return t;
}

XMMATRIX Transform::GetWorldTranslationMatrix()
{
	XMMATRIX worldSRT = GetLocalTranslationMatrix();
	for (Transform* p = m_parent; p != nullptr; p = p->m_parent)
	{
		worldSRT = p->GetLocalTranslationMatrix()*worldSRT;
	}
	return worldSRT;
}

void Transform::GetWorldSRT(XMVECTOR & scale, XMVECTOR & rotation, XMVECTOR & position)
{
	XMMATRIX worldSRT = GetLocalTranslationMatrix();
	for (Transform* p = m_parent; p != nullptr; p = p->m_parent)
	{
		worldSRT = p->GetLocalTranslationMatrix()*worldSRT;
	}
	XMMatrixDecompose(&scale, &rotation, &position, worldSRT);
}

void Transform::SetWorldSRT(FXMVECTOR scale, FXMVECTOR rotation, FXMVECTOR position)
{
	XMMATRIX parentSRT = XMMatrixIdentity();
	for (Transform* p = m_parent; p != nullptr; p = p->m_parent)
	{
		parentSRT = p->GetLocalTranslationMatrix()*parentSRT;
	}
	XMMATRIX localSRT = XMMatrixScalingFromVector(scale)*XMMatrixRotationQuaternion(rotation)*XMMatrixTranslationFromVector(position);
	localSRT = XMMatrixInverse(nullptr, parentSRT)*localSRT;
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

}

void Transform::SetParent(Transform * parent)
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);
	if (m_parent == nullptr)
	{
		Scene* s = Scene::GetCurrentScene();
		s->RemoveRootGameObject(gameobject());
	}
	else
	{
		for (auto iter = m_parent->m_children.begin(); iter != m_parent->m_children.end(); iter++)
		{
			if (*iter = this)
			{
				m_parent->m_children.erase(iter);
				break;
			}
		}
	}

	m_parent = parent;
	if (parent == nullptr)
	{
		Scene* s = Scene::GetCurrentScene();
		s->AddRootGameObject(gameobject());
	}
	else
	{
		parent->m_children.push_back(this);
	}
	SetWorldSRT(s, r, t);
}

void Transform::AddChild(Transform * child, int index)
{
	XMVECTOR s, r, t;
	GetWorldSRT(s, r, t);

	if (child->m_parent == nullptr)
	{
		Scene* s = Scene::GetCurrentScene();
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

	child->m_parent = this;
	if (index < 0||index>=m_children.size())
		m_children.push_back(child);
	else
	{
		auto iter = m_children.begin();
		iter += index;
		m_children.insert(iter, child);
	}

	SetWorldSRT(s, r, t);
}

Component* Transform::Clone()
{
	Transform* copy = new Transform();
	for (int i = 0; i < m_children.size(); i++)
	{
		GameObject* child = new GameObject(m_children[i]->gameobject());
		child->GetTransform()->SetParent(this);
	}
	copy->localPosition = localPosition;
	copy->localRotation = localRotation;
	copy->localScale = localScale;
	return static_cast<Component*>(copy);
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