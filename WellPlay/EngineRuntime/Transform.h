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
	XMVECTOR GetLocalPosition();
	XMVECTOR GetLocalRotation();
	XMFLOAT3 GetLocalEulerAngles();
	XMVECTOR GetLocalScale();
	XMMATRIX GetLocalTranslationMatrix();
	XMMATRIX GetWorldTranslationMatrix();
	void GetWorldSRT(XMVECTOR& scale, XMVECTOR& rotation, XMVECTOR& position);
	void SetWorldSRT(FXMVECTOR scale, FXMVECTOR rotation, FXMVECTOR position);

	void SetLocalPosition(FXMVECTOR position);
	void SetLocalScale(FXMVECTOR localscale);
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
	XMFLOAT3 localPosition;
	XMFLOAT4 localRotation;
	XMFLOAT3 localScale;

	Transform* m_parent;
	std::vector<Transform*> m_children;

	Transform();
	virtual ~Transform();
	
	virtual Component* Clone()override;
};