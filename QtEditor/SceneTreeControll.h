#pragma once
#include <qobject.h>
#include <qdockwidget.h>
#include "SceneModel.h"
#include <qtreeview.h>

class SceneTreeControll :public QObject
{
	Q_OBJECT

public:
	explicit SceneTreeControll(QDockWidget *target);
	~SceneTreeControll();

signals:
	void OnAddGameObject(QVector<int>, QString);
	void OnRemoveGameObject(QVector<int>, QString);
	void OnMoveGameObject(QVector<int>, QVector<int>, int);
	void LoadScene();

private:
	void SelectGameObject(const QModelIndex &select);

	QDockWidget* scenetree;
	SceneModel* model;
	QTreeView* treeview;

	std::weak_ptr<GameObject> currentSelect;
};