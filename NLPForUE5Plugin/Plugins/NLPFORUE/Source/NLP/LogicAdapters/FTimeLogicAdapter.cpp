#include "FTimeLogicAdapter.h"
#include <codecvt>

#include "Interfaces/Interface_BoneReferenceSkeletonProvider.h"

FTimeLogicAdapter::FTimeLogicAdapter()
{
	NLOG(LOGNLP,Log,TEXT("FTimeLogicAdapter constructed"));
}

FTimeLogicAdapter& FTimeLogicAdapter::Get()
{
	static FTimeLogicAdapter Instance;
	return Instance;
}

FTimeLogicAdapter::~FTimeLogicAdapter()
{
	NLOG(LOGNLP,Log,TEXT("FTimeLogicAdapter destructed"));
}

bool FTimeLogicAdapter::Process(const std::string& Input, std::string& Output)
{
	// 判断文本是否是在问时间问题
	std::string MatchStr;
	EAskType AskType;
	if(IsAskTime(Input,MatchStr,AskType))
	{
		// 根据匹配文本生成对应时间
		std::tm Tm = GetTimeFromText(MatchStr);
		// 生成默认回答
		Output = SpawnAnswer(Tm,AskType);
		return true;
	}
	else
	{
		NLOG(LOGNLP,Log,TEXT("不是询问时间问题"));
	}
	return false;
}

bool FTimeLogicAdapter::IsAskTime(const std::string& Text,std::string& MatchStr,EAskType& AskType)
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
	std::wstring_convert<std::codecvt_utf8<wchar_t>> Converter;
	std::wstring WText = Converter.from_bytes(Text);
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
			MatchStr = Converter.to_bytes(Str);
			AskType = Date;
			return true;
		}
	}
	else if(std::regex_search(WText,Matchs,PattrenWeek))
	{
		if(!Matchs.empty())
		{
			std::wstring Str = Matchs[0];
			MatchStr = Converter.to_bytes(Str);
			AskType = Week;
			return true;
		}
	}
	else if(std::regex_search(WText,Matchs,PattrenTime))
	{
		if(!Matchs.empty())
		{
			std::wstring Str = Matchs[0];
			MatchStr = Converter.to_bytes(Str);
			AskType = Time;
			return true;
		}
	}
	return false;
}

std::tm FTimeLogicAdapter::GetTimeFromText(const std::string& Text)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> Converter;
	std::wstring WText = Converter.from_bytes(Text);
	std::wregex PattrenT(L"([大]?[前昨今明后]天|现在)");
	std::wregex PattrenNum(L"((\\d+年)?((?:[1-9]|1[0-2])月)?(?:1[0-9]|2[0-9]|3[0-1]|[1-9])[日号])");
	std::wregex PattrenCN(L"(([零一二三四五六七八九]+年)?([零一二三四五六七八九十]+月)?[零一二三四五六七八九十]+[日号])");
	std::wsmatch Matchs;
	std::tm Tm = {0};
	// 如果日期是时间代词
	if(std::regex_search(WText,Matchs,PattrenT) && Matchs.size() > 0)
	{
		std::tm TimePeriod = {0};
		std::map<std::wstring,int> TimePeriodMap = {
			{L"大前天",-3},{L"前天",-2},{L"昨天",-1},{L"今天",0},
			{L"现在",0},{L"明天",1},{L"后天",2},{L"大后天",3}
		};
		Tm = GlobalManager::GetNowLocalTime();
		if(TimePeriodMap[Matchs[0]] < 0)
		{
			TimePeriod.tm_mday = -TimePeriodMap[Matchs[0]];
			Tm = GlobalManager::TimeOperator(Tm, TimePeriod,GlobalManager::OpeTag::Sub);
		}
		else
		{
			TimePeriod.tm_mday = TimePeriodMap[Matchs[0]];
			Tm = GlobalManager::TimeOperator(Tm, TimePeriod);
		}
	}
	// 如果日期是阿拉伯数字格形式，如：2023年11月21日
	else if(std::regex_search(WText,Matchs,PattrenNum))
	{
		std::vector<std::string> Words;
		std::string Tmp = Converter.to_bytes(Matchs[0]);
		GlobalManager::jieba.CutHMM(Tmp,Words);
		Tm = GlobalManager::GetNowLocalTime();
		if(Words.size() == 6)
		{
			Tm.tm_year = GlobalManager::IsNumber(Words[0])?std::stoi(Words[0]):0;
			Tm.tm_mon = GlobalManager::IsNumber(Words[2])?std::stoi(Words[2]):0;
			Tm.tm_mday = GlobalManager::IsNumber(Words[4])?std::stoi(Words[4]):0;
		}
		else if(Words.size() == 4)
		{
			Tm.tm_mon = GlobalManager::IsNumber(Words[0])?std::stoi(Words[0]):0;
			Tm.tm_mday = GlobalManager::IsNumber(Words[2])?std::stoi(Words[2]):0;
		}
		else if(Words.size() == 2)
		{
			Tm.tm_mday = GlobalManager::IsNumber(Words[0])?std::stoi(Words[0]):0;
		}
		else
		{
			NLOG(LOGNLP,Error,TEXT("Date format error : %s  Words.size=%d"),*TOFS(Text),(int)Words.size());
		}
	}
	// 如果日期是中文数字形式，如：二零二三年十一月二十一日
	else if(std::regex_search(WText,Matchs,PattrenCN))
	{
		std::vector<std::string> Words = GlobalManager::SplitTextToWord(Text);
		std::vector<std::string> NumWords;
		std::string NumWord;
		Tm = GlobalManager::GetNowLocalTime();
		for(int i=0;i<Words.size();++i)
		{
			if(GlobalManager::IsNumber(Words[i]))
			{
				NumWord += Words[i];
			}
			else
			{
				if(NumWord != "")
				{
					NumWords.push_back(NumWord);
					NumWord = "";
				}
				// 只有一句话中日期中的"日"或“号”才能作为结束标志
				if(Words[i] == "日" || Words[i] == "号")
				{
					if(i - 1 > 0)
					{
						if(GlobalManager::IsNumber(Words[i-1])) break;
					}
				}
			}
		}
		if(NumWords.size() == 3)
		{
			Tm.tm_year = GlobalManager::StrValueToIntValue(NumWords[0]);
			Tm.tm_mon = GlobalManager::StrValueToIntValue(NumWords[1]);
			Tm.tm_mday = GlobalManager::StrValueToIntValue(NumWords[2]);
		}
		else if(NumWords.size() == 2)
		{
			Tm.tm_mon = GlobalManager::StrValueToIntValue(NumWords[0]);
			Tm.tm_mday = GlobalManager::StrValueToIntValue(NumWords[1]);
		}
		else if(NumWords.size() == 1)
		{
			Tm.tm_mday = GlobalManager::StrValueToIntValue(NumWords[0]);
		}
		else
		{
			NLOG(LOGNLP,Error,TEXT("Date format error : %s NumWords.size=%d"),*TOFS(Text),(int)NumWords.size());
		}
	}
	return Tm;
}

std::string FTimeLogicAdapter::SpawnAnswer(const std::tm& Time, const EAskType AskType)
{
	std::string Rel;
	std::map<int,std::string> WeekMap = {
		{1,"星期一"},{2,"星期二"},{3,"星期三"},{4,"星期四"},
		{5,"星期五"},{6,"星期六"},{0,"星期天"}
	};
	switch (AskType)
	{
	case EAskType::Date:
		// 取标准时间格式的日期部分
		Rel = GlobalManager::TmToString(Time).substr(0,10);
		break;
	case EAskType::Week:
		// 蔡勒公式计算星期 W=(Y+Y/4+C/4-2*C+26*(M+1)/10+D-1)%7
		// W:星期，Y:年份，年的后两位数，C:世纪，年的前两位数，M:月份，D:日期
		int C,Y,M,D,W,N;
		N = std::floor(std::log10(Time.tm_year))+1;
		C = Time.tm_year/pow(10.0,N-2);
		Y = Time.tm_year%100;
		M = Time.tm_mon;
		D = Time.tm_mday;
		W = (Y+Y/4+C/4-2*C+26*(M+1)/10+D-1)%7;
		Rel = WeekMap[W];
		break;
	case EAskType::Time:
		std::string TimeStr = GlobalManager::TmToString(Time);
		// 取标准使劲啊格式的时间部分
		Rel = TimeStr.substr(11,TimeStr.length());
		break;
	}
	return Rel;
}
