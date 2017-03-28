#pragma once

#include "Model.h"
#include "stdafx.h"

#pragma comment(lib, "libfbxsdk-md.lib")

namespace FBXImport 
{
	std::shared_ptr<Model> ImportModel(std::string path);
}