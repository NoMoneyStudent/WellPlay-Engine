#pragma once
#include "stdafx.h"

typedef std::string ByteArray;
typedef std::wstring WcharArray;

namespace FileUtility
{
	std::wstring OpenFile();
	void SaveFile();
	void OpenFolder();
	std::string GetExtension(const std::string& path);
	std::string GetExtension(const std::wstring& path);
	std::string GetFileName(const std::string& path);
	std::string GetFileName(const std::wstring& wpath);
	ByteArray ReadFileByte(const std::string& path);
	WcharArray ReadFileWchar(const std::string& path);
	const unsigned char* ReadTextureStbi(const std::string path, int& w, int& h, int& channels);
	const unsigned char* ReadTextureDDS(const std::string path, int& size);
}