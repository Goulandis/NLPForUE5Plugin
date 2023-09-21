#ifndef GLOBALMANAGER
#define GLOBALMANAGER

#include "cppjieba/Jieba.hpp"
#include "NLP/Preprossors/Define.h"
#include "NLP/LogicAdapters/Define.h"
#include <regex>

namespace GlobalManager
{
	const std::string ResourcePath = TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + RESOURCE_PATH));
	// 结巴分词库全局实例
	inline cppjieba::Jieba jieba(
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + SOURCE_PATH + DICT_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + SOURCE_PATH + HMM_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + SOURCE_PATH + USER_DICT_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + SOURCE_PATH + IDF_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + SOURCE_PATH + STOP_WORD_PATH))
	);

	// 判断字符串是否匹配数学算式
	inline bool RegexMathFormulas(const std::string& Text)
	{
		std::regex rp(R"((\d+\s*[\+\-\*/\^%]\s*)+\d+)");
		return std::regex_search(Text,rp);
	}

	inline bool IsNumber(const std::string& Word)
	{
		
		//return std::regex_match(Word,rp);
		std::vector<std::pair<std::string,std::string>> Tagers;
		jieba.Tag(Word,Tagers);
		bool Rel = false;
		std::regex ExcPattren(R"(一下|一次|多少|余|二次|三次|几)");
		std::regex NumPattren(R"(\d+)");
		for(std::pair<std::string,std::string> Pai : Tagers)
		{
			if((Pai.second == "m" && !std::regex_search(Pai.first,ExcPattren)) || std::regex_search(Pai.first,NumPattren))
			{
				Rel = true;
			}
			else
			{
				Rel = false;
				break;
			}
		}
		return Rel;
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

	inline FString Log(const std::string& Str)
	{
		return FString(UTF8_TO_TCHAR(Str.c_str()));
	}
	
}

#endif

