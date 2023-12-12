#ifndef LOG_LOGNLP
#define LOG_LOGNLP

#include <comdef.h>
#include <windows.h>
#include <string>

DECLARE_LOG_CATEGORY_EXTERN(LOGNLP, All, All)
#define TOFS(Str) FString(UTF8_TO_TCHAR(Str.c_str()))
#define WSTOFS(WStr) FString(WStr.c_str())
#define PROJECTPLUGINDIR FPaths::ProjectPluginsDir()
#define TOUTF8(Param) TCHAR_TO_UTF8(Param)
#define NLOG(CategoryName,Verbosity,Format,...) UE_PRIVATE_LOG(PREPROCESSOR_NOTHING, constexpr, CategoryName, Verbosity, Format, ##__VA_ARGS__)
#endif

namespace GlobalManager
{
	// 虚幻引擎系统体系下的资源路径
	const std::string RESOURCE_ABSOLUTE_PATH = TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + "NLPFORUE/Resources/"));

	std::string WString2String(const std::wstring& wstr) {
		std::string strLocale = setlocale(LC_ALL, "");   // use the default locale set in the environment
		const wchar_t* wchSrc = wstr.c_str();
		size_t nWStr = wcstombs(NULL, wchSrc, 0) + 1;    // get the string len, zh has two byte
		char* chDest = new char[nWStr];
		memset(chDest, 0, nWStr);                        // is chDest[nWStr - 1] = 0;
		wcstombs(chDest, wchSrc, nWStr);                 // translates wide characters from the sequence pointed by src to the multibyte equivalent sequence
		std::string strRes = chDest;
		delete[] chDest;
		setlocale(LC_ALL, strLocale.c_str());

		bool bUtf8 = IsUtf8((unsigned char*)(strRes.c_str()), strRes.size());
		if (bUtf8) {
			std::string strGbk;
			char c[MAX_PATH_LEN];
			strcpy(c, strRes.c_str());
			strGbk = Utf8ToGbk(c);
			return strGbk;
		}
		return strRes;
	}
}

