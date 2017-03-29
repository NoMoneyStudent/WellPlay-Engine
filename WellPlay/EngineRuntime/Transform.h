#pragma once
#include "Component.h"
#include "GameObject.h"
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

class Transform:public Component
{
	friend class GameObject;
public:
	XMVECTOR GetPosition();
	XMVECTOR GetLocalPosition();
	XMVECTOR GetRotation();
	XMVECTOR GetLocalRotation();
	XMFLOAT3 GetEulerAngles();
	XMFLOAT3 GetLocalEulerAngles();
	XMVECTOR GetScale();
	XMVECTOR GetLocalScale();

	void Translate(FXMVECTOR distance);
	void Translate(float x,float y,float z);
	void SetPositon(FXMVECTOR position);
	void SetLocalposition(FXMVECTOR position);
	void SetScale(FXMVECTOR scale);
	void SetLocalScale(FXMVECTOR localscale);
	void RotateX(float angles);
	void RotateY(float angles);
	void RotateZ(float angles);
	void SetRotation(FXMVECTOR rotation);
	void SetLocalRotation(FXMVECTOR rotation);

	XMVECTOR GetRight();
	XMVECTOR GetUp();
	XMVECTOR GetForward();
	XMVECTOR GetBehind();
	XMVECTOR GetLeft();
	XMVECTOR GetDown();

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
	virtual ~Transform();
	
	virtual Component* Clone()override;
};