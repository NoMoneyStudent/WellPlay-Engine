#pragma once
#include "stdafx.h"

typedef std::shared_ptr<std::vector<byte> > ByteArray;

namespace FileUtility
{
	std::wstring OpenFile();
	void SaveFile();
	void OpenFolder();
	std::string GetExtension(const std::string& path);
	std::string GetExtension(const std::wstring& path);
	ByteArray ReadFileHelper(const std::string& fileName);
	const unsigned char* ReadTextureStbi(const std::string path, int& w, int& h, int& channels);
	const unsigned char* ReadTextureDDS(const std::string path, int& size);
}