#ifndef GLOBALMANAGER
#define GLOBALMANAGER

#include "cppjieba/Jieba.hpp"
#include "NLP/Preprossors/Define.h"
#include <regex>

namespace GlobalManager
{
	// 结巴分词库全局实例
	inline cppjieba::Jieba jieba(
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + DICT_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + HMM_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + USER_DICT_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + IDF_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + STOP_WORD_PATH))
	);

	// 判断字符串是否匹配数学算式
	inline bool RegexMathFormulas(const std::string& Text)
	{
		std::regex rp(R"((\d+\s*[\+\-\*/\^x]\s*)+\d+)");
		return std::regex_search(Text,rp);
	}

	inline bool IsNumber(const std::string& Word)
	{
		std::regex rp(R"(0|1|2|3|4|5|6|7|8|8|零|一|二|三|四|五|六|七|八|九|十)");
		return std::regex_match(Word,rp);
	}

	inline bool IsChinese(const std::string& Text)
	{
		const char* TChar = Text.data();
		char Chr;
		while(true)
		{
			Chr = *TChar++;
			// 如果字符串直到结尾都没有出现中文字符，则认为字符串为其他语言
			if(Chr == 0)
			{
				break;
			}
			// 如果字符串中出现了中文，则认为字符串为中文
			if((Chr & 0x80) && (*TChar & 0x80))
			{
				return true;
			}
		}
		return false;
	}
}

// Global debug functions
namespace DebugLog
{
	
	inline void PrintVec(const std::vector<std::string>& Vec,std::string Flag)
	{
		for (int i=0;i<Vec.size();++i)
		{
			UE_LOG(LogTemp,Log,TEXT("%s:%s"),*FString(UTF8_TO_TCHAR(Flag.c_str())),*FString(UTF8_TO_TCHAR(Vec[i].c_str())));
		}
	}
	
}

#endif

