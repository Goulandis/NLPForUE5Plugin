#ifndef LOG_LOGNLP
#define LOG_LOGNLP

#include <comdef.h>
#include <windows.h>
#include <string>

DECLARE_LOG_CATEGORY_EXTERN(LOGNLP, All, All)
#define TOFS(Str) FString(UTF8_TO_TCHAR(Str.c_str()))
#define WSTOFS(WStr) FString(WStr.c_str())
#define CHTOFS(Chr) FString(Chr)
#define PROJECTPLUGINDIR FPaths::ProjectPluginsDir()
#define TOUTF8(Param) TCHAR_TO_UTF8(Param)
#define NLOG(CategoryName,Verbosity,Format,...) UE_PRIVATE_LOG(PREPROCESSOR_NOTHING, constexpr, CategoryName, Verbosity, Format, ##__VA_ARGS__)
#endif

namespace GlobalManager
{
	// 虚幻引擎系统体系下的资源路径
	const std::string RESOURCE_ABSOLUTE_PATH = TCHAR_TO_UTF8(*(PROJECTPLUGINDIR + "NLPFORUE/Resources/"));
}

