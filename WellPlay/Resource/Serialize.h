#pragma once
#include "EngineRuntime\GameObject.h"
#include "Resource\Model.h"

void MakeAssets(std::shared_ptr<Assets> asset);
void ReadAssets(const std::string& path);
void MakePrefab(std::shared_ptr<GameObject> target);
void ReadPrefab(const std::string& path);