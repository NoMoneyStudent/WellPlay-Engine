#pragma once
#include "Component.h"
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

class Transform:public Component
{
	friend class GameObject;
public:
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLocalPosition();
	XMFLOAT4 GetRotation();
	XMFLOAT4 GetLocalRotation();
	XMFLOAT3 GetEulerAngles();
	XMFLOAT3 GetLocalEulerAngles();
	XMFLOAT3 GetScale();
	XMFLOAT3 GetLocalScale();

	void Translate(XMFLOAT3 distance);
	void Translate(float x,float y,float z);
	void SetPositon(XMFLOAT3 position);
	void SetLocalposition(XMFLOAT3 position);
	void SetScale(XMFLOAT3 scale);
	void SetLocalScale(XMFLOAT3 localscale);
	void RotateX(float angles);
	void RotateY(float angels);
	void RotateZ(float angels);
	void SetRotation(XMFLOAT4 rotation);
	void SetLocalRotation(XMFLOAT4 rotation);

	XMFLOAT3 GetRight();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetForward();
	XMFLOAT3 GetBehind();
	XMFLOAT3 GetLeft();
	XMFLOAT3 GetDown();

	Transform* GetParent() { return m_parent; }
	std::vector<Transform*> GetChildren() { return m_children; }
	void SetParent(Transform* parent);
	void AddChild(Transform* child, int index = -1);

private:
	XMFLOAT3 worldPosition;
	XMFLOAT4 worldRotation;
	XMFLOAT3 worldScale;

	Transform* m_parent;
	std::vector<Transform*> m_children;

	Transform();
	~Transform();
};

Transform::Transform()
{
}

Transform::~Transform()
{
}