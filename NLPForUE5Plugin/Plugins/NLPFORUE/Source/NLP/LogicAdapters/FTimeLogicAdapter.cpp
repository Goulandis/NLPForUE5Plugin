#include "FTimeLogicAdapter.h"
#include <codecvt>

FTimeLogicAdapter::FTimeLogicAdapter()
{
	NLOG(LOGNLP,Log,TEXT("FTimeLogicAdapter constructed"));
}

FTimeLogicAdapter& FTimeLogicAdapter::CreateInstance()
{
	static FTimeLogicAdapter Instance;
	return Instance;
}

FTimeLogicAdapter::~FTimeLogicAdapter()
{
	NLOG(LOGNLP,Log,TEXT("FTimeLogicAdapter destructed"));
}

bool FTimeLogicAdapter::Process(const std::string& Input, const std::string& Output)
{
	// 判断文本是否是在问时间问题
	std::string MatchStr;
	if(IsAskTime(Input,MatchStr))
	{
		// 根据匹配文本生成对应时间
		
	}
	return false;
}

bool FTimeLogicAdapter::IsAskTime(const std::string& Text,std::string& MatchStr)
{
	/*
	 * 问日期：
	 * [大]?[前昨今明后]天是几号|[大]?[前昨今明后]天的日期
	 * 现在是几号|现在的日期
	 * 问星期：
	 * [大]?[前昨今明后]天是星期几|[大]?[前昨今明后]天是周几
	 * (\\d+年)?((?:[1-9]|1[0-2])月)?(?:1[0-9]|2[0-9]|3[0-1]|[1-9])[日号]是周几|(\\d+年)?((?:[1-9]|1[0-2])月)?(?:1[0-9]|2[0-9]|3[0-1]|[1-9])[日号]是星期几
	 * ([零一二三四五六七八九]+年)?([零一二三四五六七八九十]+月)?[零一二三四五六七八九十]+[日号]是周几|([零一二三四五六七八九]+年)?([零一二三四五六七八九十]+月)?[零一二三四五六七八九十]+[日号]是星期几
	 * 问时间：
	 * 现在是几点[钟]?
	 * 现在的时间
	 */
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring WText = converter.from_bytes(Text);
	std::wregex PattrenDate(L"([大]?[前昨今明后]天是几号|([大]?[前昨今明后]天的日期是(几号|多少|什么))|现在是几号|(现在的日期是(几号|多少|什么)))");
	std::wstring WeekRegexStr1 = L"(([大]?[前昨今明后]天是(星期|周)几)|(现在是(星期|周)几))";
	std::wstring WeekRegexStr2 = L"(((\\d+年)?((?:[1-9]|1[0-2])月)?(?:1[0-9]|2[0-9]|3[0-1]|[1-9])[日号]是)(星期|周)几)";
	std::wstring WeekRegexStr3 = L"((([零一二三四五六七八九]+年)?([零一二三四五六七八九十]+月)?[零一二三四五六七八九十]+[日号]是)(星期|周)几)";
	std::wregex PattrenWeek;
	PattrenWeek = WeekRegexStr1 + L"|" + WeekRegexStr2 + L"|" + WeekRegexStr3;
	std::wregex PattrenTime(L"(现在是几点[钟]?|(现在的时间是(什么|多少|几点|几点钟)))");
	std::wsmatch Matchs;
	if(std::regex_search(WText,Matchs,PattrenDate))
	{
		if(!Matchs.empty())
		{
			std::wstring Str = Matchs[0];
			MatchStr = converter.to_bytes(Str);
			return true;
		}
	}
	else if(std::regex_search(WText,Matchs,PattrenWeek))
	{
		if(!Matchs.empty())
		{
			std::wstring Str = Matchs[0];
			MatchStr = converter.to_bytes(Str);
			return true;
		}
	}
	else if(std::regex_search(WText,Matchs,PattrenTime))
	{
		if(!Matchs.empty())
		{
			std::wstring Str = Matchs[0];
			MatchStr = converter.to_bytes(Str);
			return true;
		}
	}
	return false;
}

std::tm FTimeLogicAdapter::GetTimeFromText(const std::string& Text)
{
}