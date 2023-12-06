#ifndef LOG_LOGNLP
#define LOG_LOGNLP
#include <string>
#include <vector>
DECLARE_LOG_CATEGORY_EXTERN(LOGNLP, All, All)
#define TOFSTR(Str) FString(UTF8_TO_TCHAR(Str.c_str()))
#endif

namespace GlobalManager
{
	// 虚幻引擎系统体系下的资源路径
	const std::string RESOURCE_PATH = TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + "NLPFORUE/Resources/"));

	class UEManager
	{
	public:
	
	};
}

