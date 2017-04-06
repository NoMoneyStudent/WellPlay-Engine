#pragma once
#include "EngineRuntime\GameObject.h"

void MakePrefab(std::shared_ptr<GameObject> target);
void ReadPrefab(const std::string& path);