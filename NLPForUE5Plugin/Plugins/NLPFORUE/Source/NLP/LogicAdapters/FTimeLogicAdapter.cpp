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

	return false;
}

bool FTimeLogicAdapter::IsAskTime(const std::string& Text)
{
	/*
	 * 问日期：
	 * [大]?[前昨今明后]天是几号|现在是几号|[大]?[前昨今明后]天的日期是多少|[大]?[前昨今明后]天的日期是几号|[大]?[前昨今明后]天的日期是什么
	 * 问星期：
	 * [大]?[前昨今明后]天是星期几|[大]?[前昨今明后]天是周几
	 * \d+年?\b(?:[1-9]|1[0-2])\b月?\b(?:[1-9]|1\d|2[0-9]|3[0-1])\b[日号]是星期几|\d+年?\b(?:[1-9]|1[0-2])\b月?\b(?:[1-9]|1\d|2[0-9]|3[0-1])\b[日号]是周几
	 * [零一二三四五六七八九]+年?[零一二三四五六七八九十]+月?[零一二三四五六七八九十]+][日|号]是星期几|[零一二三四五六七八九]+年?[零一二三四五六七八九十]+月?[零一二三四五六七八九十]+][日|号]是周几
	 * 问时间：
	 * 现在是几点[钟]
	 */
	// [大前天、前天、昨天、今天、现在、明天、后天、大后天]是[几号、星期几、周几]
	// 现在是几点
	// 现在的时间
	// xxxx是周几

	
	std::wstring WText = StringToWideString(Text);
	std::wregex PattrenDate(L"([大]?[前昨今明后]天是几号|现在是几号|[大]?[前昨今明后]天的日期是多少|[大]?[前昨今明后]天的日期是几号|[大]?[前昨今明后]天的日期是什么)");
	std::wregex PattrenWeek(L"([大]?[前昨今明后]天是星期几|[大]?[前昨今明后]天是周几)");
	// [\d+年\b(?:[1-9]|1[0-2])\b月\b(?:[1-9]|1\d|2[0-9]|3[0-1])\b|[零一二三四五六七八九]+年[零一二三四五六七八九十]+月[零一二三四五六七八九十]+][日|号]是[周几|星期几]
	std::wsmatch Matchs;
	if(std::regex_search(WText,Matchs,PattrenDate))
	{
		std::wstring Str = Matchs[0];
		NLOG(LogTemp,Log,TEXT("%s"),*WSTOFS(Str));
	}
	
	std::vector<std::string> Pattrens = {
		R"([大]?[前昨今明后]天是几号)",
	};
	
	return false;
}

std::wstring FTimeLogicAdapter::StringToWideString(const std::string& Str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> Conv;
	return Conv.from_bytes(Str);
}
