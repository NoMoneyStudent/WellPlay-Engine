#include "stdafx.h"
#include "Transform.h"
#include "Scene.h"

Transform::Transform():
	worldPosition(XMFLOAT3(0,0,0)),
	worldRotation(XMFLOAT4(0, 0, 0, 1)),
	worldScale(XMFLOAT3(1, 1, 1))
{
}

Transform::~Transform()
{
}

void Transform::SetParent(Transform * parent)
{
	for (auto iter = m_parent->m_children.begin(); iter != m_parent->m_children.end(); iter++)
	{
		if (*iter = this)
		{
			m_parent->m_children.erase(iter);
			break;
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
}

void Transform::AddChild(Transform * child, int index)
{
	for (auto iter = child->m_parent->m_children.begin(); iter != child->m_parent->m_children.end(); iter++)
	{
		if (*iter = child)
		{
			child->m_parent->m_children.erase(iter);
			break;
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
}

Component* Transform::Clone()
{
	Transform* copy = new Transform();
	for (int i = 0; i < m_children.size(); i++)
	{
		GameObject* child = new GameObject(m_children[i]->gameobject());
		child->GetTransform()->SetParent(this);
	}
	copy->worldPosition = worldPosition;
	copy->worldRotation = worldRotation;
	copy->worldScale = worldScale;
	return static_cast<Component*>(copy);
}

XMVECTOR Transform::GetPosition()
{
	return XMLoadFloat3(&worldPosition);
}

XMVECTOR Transform::GetLocalPosition()
{
	if (m_parent == nullptr)
		return GetPosition();
	else
		return GetPosition() - m_parent->GetPosition();
}

XMVECTOR Transform::GetRotation()
{
	return XMLoadFloat4(&worldRotation);
}

XMVECTOR Transform::GetLocalRotation()
{
	if (m_parent == nullptr)
		return GetRotation();
	else
		return XMQuaternionMultiply(GetRotation(), XMQuaternionInverse(m_parent->GetRotation()));
}

XMFLOAT3 Transform::GetEulerAngles()
{
	return XMQuaternion2Euler(GetRotation());
}

XMFLOAT3 Transform::GetLocalEulerAngles()
{
	if (m_parent == nullptr)
		return GetEulerAngles();
	else
	{
		XMFLOAT3 myEuler = GetEulerAngles();
		XMFLOAT3 parentEuler = m_parent->GetEulerAngles();
		return XMFLOAT3(myEuler.x - parentEuler.x, myEuler.y - parentEuler.y, myEuler.z - parentEuler.z);
	}
}

XMVECTOR Transform::GetScale()
{
	return XMLoadFloat3(&worldScale);
}

XMVECTOR Transform::GetLocalScale()
{
	if (m_parent == nullptr)
		return GetScale();
	else
		return XMVectorMultiply(GetScale(), XMVectorReciprocalEst(m_parent->GetScale()));
}

void Transform::Translate(FXMVECTOR distance)
{
	XMStoreFloat3(&worldPosition, XMVectorAdd(distance,GetPosition()));
}

void Transform::Translate(float x, float y, float z)
{
	Translate(XMVectorSet(x, y, z, 0));
}

void Transform::SetPositon(FXMVECTOR position)
{
	XMStoreFloat3(&worldPosition, position);
}

void Transform::SetLocalposition(FXMVECTOR position)
{
	if (m_parent == nullptr)
		XMStoreFloat3(&worldPosition, position);
	else
		XMStoreFloat3(&worldPosition, XMVectorAdd(m_parent->GetPosition(), position));
}

void Transform::SetScale(FXMVECTOR scale)
{
	XMStoreFloat3(&worldScale, scale);
}

void Transform::SetLocalScale(FXMVECTOR localscale)
{
	if (m_parent == nullptr)
		XMStoreFloat3(&worldScale, localscale);
	else
		XMStoreFloat3(&worldScale, XMVectorMultiply(localscale, m_parent->GetScale()));
}

void Transform::RotateX(float angles)
{
	XMStoreFloat4(&worldRotation, XMQuaternionMultiply(GetRotation(), XMQuaternionRotationAxis(XMVectorSet(1, 0, 0, 0), angles)));
}

void Transform::RotateY(float angles)
{
	XMStoreFloat4(&worldRotation, XMQuaternionMultiply(GetRotation(), XMQuaternionRotationAxis(XMVectorSet(0, 1, 0, 0), angles)));
}

void Transform::RotateZ(float angles)
{
	XMStoreFloat4(&worldRotation, XMQuaternionMultiply(GetRotation(), XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), angles)));
}

void Transform::SetRotation(FXMVECTOR rotation)
{
	XMStoreFloat4(&worldRotation, rotation);
}

void Transform::SetLocalRotation(FXMVECTOR rotation)
{
	if (m_parent == nullptr)
		XMStoreFloat4(&worldRotation, rotation);
	else
		XMStoreFloat4(&worldRotation, XMQuaternionMultiply(GetRotation(), rotation));
}

XMVECTOR Transform::GetRight()
{
	return XMQuaternionRotate(XMVectorSet(1, 0, 0, 0), GetRotation());
}

XMVECTOR Transform::GetUp()
{
	return XMQuaternionRotate(XMVectorSet(0, 1, 0, 0), GetRotation());
}

XMVECTOR Transform::GetForward()
{
	return XMQuaternionRotate(XMVectorSet(0, 0, 1, 0), GetRotation());
}

XMVECTOR Transform::GetBehind()
{
	return XMQuaternionRotate(XMVectorSet(0, 0, -1, 0), GetRotation());
}

XMVECTOR Transform::GetLeft()
{
	return XMQuaternionRotate(XMVectorSet(-1, 0, 0, 0), GetRotation());
}

XMVECTOR Transform::GetDown()
{
	return XMQuaternionRotate(XMVectorSet(0, -1, 0, 0), GetRotation());
}