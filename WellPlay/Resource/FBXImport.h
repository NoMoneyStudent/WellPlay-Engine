#pragma once
#include "Model.h"
#include "stdafx.h"

#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "assimp-vc140-mt.lib")

namespace FBXImport 
{
	void ImportModel(std::string path);
}