#pragma once
#include "Component.h"
#include "Scene.h"
#include <vector>
#include <DirectXMath.h>
#include <cereal/access.hpp>
class GameObject;

using namespace DirectX;

class Transform:public Component,public std::enable_shared_from_this<Transform>
{
	friend class GameObject;
	friend class cereal::access;

public:
	Transform();
	virtual ~Transform();

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

	std::shared_ptr<Transform> GetParent() { return m_parent; }
	std::vector<std::shared_ptr<Transform>> GetChildren() { return m_children; }
	void SetParent(std::shared_ptr<Transform> parent);
	void AddChild(std::shared_ptr<Transform> child, int index = -1);

private:
	XMFLOAT3 localPosition;
	XMFLOAT4 localRotation;
	XMFLOAT3 localScale;

	std::shared_ptr<Transform> m_parent;
	std::vector<std::shared_ptr<Transform>> m_children;
	
	virtual Component* Clone()override;
#pragma region –Ú¡–ªØ
	template<class Archive>
	void save(Archive & archive) const
	{
		archive(localPosition, localRotation, localScale);
		archive(m_parent, m_children);
		archive(cereal::base_class<Component>(this));
	}

	template<class Archive>
	void load(Archive & archive)
	{
		archive(localPosition, localRotation, localScale);
		archive(m_parent, m_children);
		archive(cereal::base_class<Component>(this));
		if (m_parent == nullptr)
			Scene::GetCurrentScene()->AddRootGameObject(gameobject());
	}
#pragma endregion
};