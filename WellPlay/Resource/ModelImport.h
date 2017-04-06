#pragma once
#include "Model.h"
#include "stdafx.h"

#pragma comment(lib, "assimp-vc140-mt.lib")

namespace ModelImport 
{
	void ImportModel(std::string path);
}