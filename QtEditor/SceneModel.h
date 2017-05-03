#pragma once
#include <qstandarditemmodel.h>
#include <mutex>
#include <memory>

class GameObject;

class SceneModel :public QStandardItemModel
{
	Q_OBJECT

public slots:
	void LoadScene();
	void OnAddGameObject(QVector<int>, QString);
	void OnRemoveGameObject(QVector<int>, QString);
	void OnMoveGameObject(QVector<int>, QVector<int>, int);

private:
	
};