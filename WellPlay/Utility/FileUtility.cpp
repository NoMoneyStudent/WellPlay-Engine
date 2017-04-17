#include "stdafx.h"
#include "FileUtility.h"
#include <fstream>
#include <locale>
#include <codecvt>
#include <sstream>
#include <Shlobj.h>  
#include <Commdlg.h> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"
using namespace std;

namespace FileUtility
{
	std::wstring OpenFile()
	{
		OPENFILENAME ofn = { 0 };
		TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名  
		ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小  
		ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
		ofn.lpstrFilter = TEXT("所有文件\0*.*\0C/C++ Flie\0*.cpp;*.c;*.h\0\0");//设置过滤  
		ofn.nFilterIndex = 1;//过滤器索引  
		ofn.lpstrFile = strFilename;//接收返回的文件名，注意第一个字符需要为NULL  
		ofn.nMaxFile = sizeof(strFilename);//缓冲区长度  
		ofn.lpstrInitialDir = NULL;//初始目录为默认  
		ofn.lpstrTitle = TEXT("请选择一个文件");//使用系统默认标题留空即可  
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项  
		if (GetOpenFileName(&ofn))
		{
			MessageBox(NULL, strFilename, TEXT("选择的文件"), 0);
		}
		return std::wstring(strFilename);
	}

	void SaveFile()
	{
		OPENFILENAME ofn = { 0 };
		ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小  
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//目录必须存在，覆盖文件前发出警告  
		ofn.lpstrTitle = TEXT("保存到");//使用系统默认标题留空即可  
		ofn.lpstrDefExt = TEXT("cpp");//默认追加的扩展名  
		if (GetSaveFileName(&ofn))
		{
			MessageBox(NULL, NULL, TEXT("保存到"), 0);
		}

	}

	void OpenFolder()
	{
		TCHAR szBuffer[MAX_PATH] = { 0 };
		BROWSEINFO bi = { 0 };
		bi.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄  
		bi.pszDisplayName = szBuffer;//接收文件夹的缓冲区  
		bi.lpszTitle = TEXT("选择一个文件夹");//标题  
		bi.ulFlags = BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST idl = SHBrowseForFolder(&bi);
		if (SHGetPathFromIDList(idl, szBuffer)) {
			MessageBox(NULL, szBuffer, TEXT("你选择的文件夹"), 0);
		}
		else {
			MessageBox(NULL, TEXT("请选择一个文件夹"), NULL, MB_ICONERROR);
		}
	}

	ByteArray ReadFileByte(const std::string& fileName)
	{
		std::ifstream& file = std::ifstream(fileName, std::ios::in | std::ios::binary);
		if (!file)
			return std::string();
		ByteArray byteArray;
		byteArray.reserve((UINT)file.seekg(0, std::ios::end).tellg());
		file.seekg(0, std::ios::beg);
		byteArray.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
		file.close();

		ASSERT(byteArray.size() > 0);

		return byteArray;
	}

	WcharArray ReadFileWchar(const std::string & path)
	{
		ByteArray byte = ReadFileByte(path);
		wstring_convert<codecvt_utf8<wchar_t>> conv;
		wstring wchar = conv.from_bytes(byte);
		return wchar;
	}

	/*使用时要记得Delete*/
	const unsigned char* ReadTextureStbi(const std::string path,int& w,int& h,int& channels )
	{
		std::ifstream fs(path.c_str(), std::ios::binary);
		fs.seekg(0, std::ios::end);
		int size = fs.tellg();
		char * data = new char[size + 1];
		fs.seekg(0);
		fs.read(data, size);
		fs.close();
		data[size] = 0;
		return stbi_load_from_memory((unsigned char *)data, size, &w, &h, &channels, 0);
	}
	/*使用时要记得Delete*/
	const unsigned char* ReadTextureDDS(const std::string path,int& size)
	{
		std::ifstream fs(path.data(), std::ios::binary| std::ios::in);
		if (!fs)
		{
			return nullptr;
		}

		fs.seekg(0, std::ios::end);
		int hehe = fs.tellg();
		size = hehe;
		char * data = new char[size];
		fs.seekg(0);
		fs.read(data, size);
		fs.close();
		return (unsigned char *)data;
	}

	std::string GetExtension(const std::string& path)
	{
		std::string result(std::find(path.crbegin(), path.crend(), '.').base(), path.cend());
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}

	std::string GetExtension(const std::wstring& wpath)
	{
		const std::string path = MakeStr(wpath);
		return GetExtension(path);
	}

	std::string GetFileName(const std::wstring& wpath)
	{
		const std::string path = MakeStr(wpath);
		return GetFileName(path);
	}

	std::string GetFileName(const std::string& path)
	{
		if (path.empty())
			return std::string();
		std::string result(std::find(path.crbegin(), path.crend(), '\\').base(),std::find(path.crbegin(), path.crend(), '.').base()-1);
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}
}