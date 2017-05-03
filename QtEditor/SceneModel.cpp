#include "SceneModel.h"
#include "EngineRuntime\Scene.h"
#include "EngineRuntime\GameObject.h"
#include "EngineRuntime\Transform.h"
#include <memory>
#include <vector>
#include <cassert>

using namespace std;

void LoadHelper(QStandardItem * parent, shared_ptr<GameObject> objectparent)
{
	auto children = objectparent->GetTransform().lock()->GetChildren();
	for (auto& child : children)
	{
		auto& gameobject = child->gameobject();
		QStandardItem* childitem = new QStandardItem;
		childitem->setText(QString::fromStdString(gameobject->GetName()));
		parent->appendRow(childitem);

		LoadHelper(childitem, gameobject);
	}
}

void SceneModel::LoadScene()
{
	QStandardItem* root = invisibleRootItem();
	auto rootGO = Scene::GetCurrentScene()->GetRootGameObject();
	for (auto& object : rootGO)
	{
		QStandardItem* childitem = new QStandardItem;
		childitem->setText(QString::fromStdString(object->GetName()));
		root->appendRow(childitem);

		LoadHelper(childitem, object);
	}
}

void SceneModel::OnAddGameObject(QVector<int> index, QString target)
{
	QStandardItem* root = invisibleRootItem();
	for (int i = 0; i < index.size() - 1; i++)
	{
		assert(root != nullptr);
		root = root->child(index[i]);
	}
	QStandardItem* childitem = new QStandardItem();
	childitem->setText(target);
	root->insertRow(index[index.size() - 1], childitem);
}

void SceneModel::OnRemoveGameObject(QVector<int> index, QString target)
{
	QStandardItem* root = invisibleRootItem();
	for (int i = 0; i < index.size() - 1; i++)
	{
		assert(root != nullptr);
		root = root->child(index[i]);
	}
	root->removeRow(index[index.size() - 1]);
}

void SceneModel::OnMoveGameObject(QVector<int> parent, QVector<int> child, int index)
{
	QStandardItem* root = invisibleRootItem();
	auto p = root, c = root;
	for (int i = 0; i < parent.size(); i++)
	{
		p = p->child(parent[i]);
		assert(p != nullptr);
	}
	for (int i = 0; i < child.size(); i++)
	{
		c = c->child(child[i]);
		assert(c != nullptr);
	}
	auto cp = c->parent() != nullptr ? c->parent() : root;

	auto list = cp->takeRow(c->row());
	assert(list.size() == 1);
	c = list[0];

	if (index < 0)
	{
		p->appendRow(c);
	}
	else
	{
		p->insertRow(index, c);
	}
}