#pragma once
#define F_INLINE __forceinline
#define I_CAST(x) (static_cast<int>(x))
#define B_CAST(x) (static_cast<bool>(x))
#define F_CAST(x) (static_cast<float>(x))
#define BONE_MAX 64

#include <DirectXMath.h>
#include <string.h>
#include <string>


namespace Utility
{
#include "math\yuki_math.h"
	class YQUtility
	{
	public:
		YQUtility() {}
		~YQUtility() {}
	private:
	public:
		
		static wchar_t* FindExtention(int strlen, wchar_t* str)
		{
			int n = strlen;
			int count = 0;
			while ((n--)>0)
			{
				count++;
				if (str[n] == L'.')
				{
					return &str[n + 1];
				}
			}
			count = count;
			return nullptr;
		}

		static const std::wstring ConnectFilePathAndTextureName(wchar_t* filepath, wchar_t* texture_name)
		{
			std::wstring path;
			path = filepath;

			int result = static_cast<int>(path.rfind(L"/"));
			if (result != -1)
			{
				path.erase(path.begin() + result + 1, path.end());
			}
			else
			{
				result = static_cast<int>(path.rfind(L"\\"));
				if (result != -1)
				{
					path.erase(path.begin() + result + 1, path.end());
				}
			}
			std::wstring tex_string;
			tex_string = texture_name;
			result = static_cast<int>(tex_string.rfind(L"/"));
			if (result != -1)
			{
				tex_string.erase(tex_string.begin(), tex_string.begin() + result);
			}
			else
			{
				result = static_cast<int>(tex_string.rfind(L"\\"));
				if (result != -1)
				{
					tex_string.erase(tex_string.begin(), tex_string.begin() + result + 1);
				}
			}

			std::wstring fullpath;
			fullpath = path + tex_string;
			return fullpath;

		}
		
		static std::wstring ChangeToWcharFromChar(const char* filename)
		{
			size_t len = strlen(filename);
			wchar_t w_filename[128];
			//setlocale(LC_ALL, "japanese");
			mbstowcs_s(&len, w_filename, 128, filename, _TRUNCATE);
			std::wstring out = w_filename;
			return out;
		}
		static std::string ChangeToCharFromWchar(const wchar_t* filename)
		{
			size_t len = wcslen(filename);

			char c_filename[128];


			//setlocale(LC_CTYPE, "jpn");

			wcstombs_s(&len, c_filename, 128, filename, _TRUNCATE);
			std::string out = c_filename;
			return out;

		}
	};


	//F_INLINE void OutputLog(std::wstring str)
	//{
	//	OutputDebugStringW(str.c_str());
	//}
}