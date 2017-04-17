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
		TCHAR strFilename[MAX_PATH] = { 0 };//���ڽ����ļ���  
		ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С  
		ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������  
		ofn.lpstrFilter = TEXT("�����ļ�\0*.*\0C/C++ Flie\0*.cpp;*.c;*.h\0\0");//���ù���  
		ofn.nFilterIndex = 1;//����������  
		ofn.lpstrFile = strFilename;//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL  
		ofn.nMaxFile = sizeof(strFilename);//����������  
		ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��  
		ofn.lpstrTitle = TEXT("��ѡ��һ���ļ�");//ʹ��ϵͳĬ�ϱ������ռ���  
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��  
		if (GetOpenFileName(&ofn))
		{
			MessageBox(NULL, strFilename, TEXT("ѡ����ļ�"), 0);
		}
		return std::wstring(strFilename);
	}

	void SaveFile()
	{
		OPENFILENAME ofn = { 0 };
		ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С  
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;//Ŀ¼������ڣ������ļ�ǰ��������  
		ofn.lpstrTitle = TEXT("���浽");//ʹ��ϵͳĬ�ϱ������ռ���  
		ofn.lpstrDefExt = TEXT("cpp");//Ĭ��׷�ӵ���չ��  
		if (GetSaveFileName(&ofn))
		{
			MessageBox(NULL, NULL, TEXT("���浽"), 0);
		}

	}

	void OpenFolder()
	{
		TCHAR szBuffer[MAX_PATH] = { 0 };
		BROWSEINFO bi = { 0 };
		bi.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������  
		bi.pszDisplayName = szBuffer;//�����ļ��еĻ�����  
		bi.lpszTitle = TEXT("ѡ��һ���ļ���");//����  
		bi.ulFlags = BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST idl = SHBrowseForFolder(&bi);
		if (SHGetPathFromIDList(idl, szBuffer)) {
			MessageBox(NULL, szBuffer, TEXT("��ѡ����ļ���"), 0);
		}
		else {
			MessageBox(NULL, TEXT("��ѡ��һ���ļ���"), NULL, MB_ICONERROR);
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

	/*ʹ��ʱҪ�ǵ�Delete*/
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
	/*ʹ��ʱҪ�ǵ�Delete*/
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