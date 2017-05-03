#pragma once
#include "Component.h"
#include "Scene.h"
#include <DirectXMath.h>
#include <vector>
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

	XMVECTOR GetLocalPosition() const;
	XMVECTOR GetLocalRotation() const;
	XMFLOAT3 GetLocalEulerAngles() const;
	XMVECTOR GetLocalScale() const;

	XMVECTOR GetWorldPosition() const;
	XMVECTOR GetWorldRotation() const;
	XMFLOAT3 GetWorldEulerAngles() const;
	XMVECTOR GetWorldScale() const;

	XMMATRIX GetLocalTranslationMatrix() const;
	XMMATRIX GetWorldTranslationMatrix() const;
	void GetWorldSRT(XMVECTOR& scale, XMVECTOR& rotation, XMVECTOR& position) const;
	void SetWorldSRT(FXMVECTOR scale, FXMVECTOR rotation, FXMVECTOR position);

	void SetLocalPosition(FXMVECTOR position);
	void SetLocalScale(FXMVECTOR scale);
	void SetLocalRotation(FXMVECTOR rotation);
	void SetWorldPosition(FXMVECTOR position);
	void SetWorldScale(FXMVECTOR scale);
	void SetWorldRotation(FXMVECTOR rotation);

	XMVECTOR GetRight() const;
	XMVECTOR GetUp() const;
	XMVECTOR GetForward() const;
	XMVECTOR GetBehind() const;
	XMVECTOR GetLeft() const;
	XMVECTOR GetDown() const;

	std::vector<int> GetIndexHierarchy() const;

	std::shared_ptr<Transform> GetParent() const { return m_parent; }
	std::vector<std::shared_ptr<Transform>> GetChildren() const { return m_children; }
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