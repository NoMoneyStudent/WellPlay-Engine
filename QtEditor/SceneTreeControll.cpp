#include "SceneTreeControll.h"
#include <assert.h>
#include "WellPlayMain.h"

SceneTreeControll::SceneTreeControll(QDockWidget *target):
	QObject(),
	scenetree(target),
	model(new SceneModel)
{
	treeview = scenetree->findChild<QTreeView*>("SceneTreeView");
	treeview->setModel(model);

	qRegisterMetaType<QVector<int>>("QVector<int>");

	EngineCallBack::OnLoadedScene =
		[&]() {emit this->LoadScene(); };
	EngineCallBack::OnAddGameObject = 
		[&](const GameObject& go) 
	{
		auto i = go.GetTransform().lock()->GetIndexHierarchy();
		emit this->OnAddGameObject(QVector<int>::fromStdVector(i), QString::fromStdString(go.GetName())); 
	};
	EngineCallBack::OnRemoveGameObject =
		[&](const GameObject& go) 
	{
		auto i = go.GetTransform().lock()->GetIndexHierarchy();
		emit this->OnRemoveGameObject(QVector<int>::fromStdVector(i), QString::fromStdString(go.GetName()));
	};
	EngineCallBack::OnMoveGameObject =
		[&](const GameObject& parent, const GameObject& child,int index)
	{
		auto p = parent.GetTransform().lock()->GetIndexHierarchy();
		auto c = child.GetTransform().lock()->GetIndexHierarchy();
		emit this->OnMoveGameObject(
			QVector<int>::fromStdVector(p), 
			QVector<int>::fromStdVector(c),
			index);
	};

	connect(treeview, &QTreeView::clicked, this, &SceneTreeControll::SelectGameObject);
	connect(this, &SceneTreeControll::OnAddGameObject, model, &SceneModel::OnAddGameObject);
	connect(this, &SceneTreeControll::OnRemoveGameObject, model, &SceneModel::OnRemoveGameObject);
	connect(this, &SceneTreeControll::OnMoveGameObject, model, &SceneModel::OnMoveGameObject);
	connect(this, &SceneTreeControll::LoadScene, model, &SceneModel::LoadScene);
}

SceneTreeControll::~SceneTreeControll()
{
}

void SceneTreeControll::SelectGameObject(const QModelIndex & select)
{
	QModelIndex index = select;
	std::vector<int> indexarray;
	while (index.isValid())
	{
		indexarray.push_back(index.row());
		index = index.parent();
	}
	std::reverse(indexarray.begin(), indexarray.end());
	assert(indexarray.size() > 0);

	auto lk = Interrupt();
	auto rootobject = Scene::GetCurrentScene()->GetRootGameObject();
	auto trans = rootobject[indexarray[0]]->GetTransform().lock();
	for (int i = 1; i < indexarray.size(); i++)
	{
		trans = trans->GetChildren()[indexarray[i]];
	}
	auto go = trans->gameobject();
	std::string m_name = go->GetName();
	auto transfrom = go->GetTransform().lock();
	auto p = transfrom->GetLocalPosition();
	auto r = transfrom->GetLocalEulerAngles();
	auto s = transfrom->GetLocalScale();
	currentSelect = go;
	
}
