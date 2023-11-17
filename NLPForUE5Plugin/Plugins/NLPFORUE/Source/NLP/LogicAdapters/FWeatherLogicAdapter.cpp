#include "FWeatherLogicAdapter.h"
#include <fstream>
#include "NLP/Common/GlobalManager.h"

FWeatherLogicAdapter::FWeatherLogicAdapter()
{
	InitCityAdcode();
	UE_LOG(LOGNLP,Log,TEXT("FWeatherLogicAdapter constructed"));
}

FWeatherLogicAdapter& FWeatherLogicAdapter::CreateInstance()
{
	static FWeatherLogicAdapter Instance;
	return Instance;
}

FWeatherLogicAdapter::~FWeatherLogicAdapter()
{
	UE_LOG(LOGNLP,Log,TEXT("FWeatherLogicAdapter destructed"));
}

bool FWeatherLogicAdapter::Process(const std::string& Input, std::string& Output)
{
	// 判断文本是否是在问天气信息
	std::string MatchText;
	if(IsAskWeather(Input,MatchText))
	{
		// 提取城市
		std::string City = GetCityFromText(MatchText);
		// 提取城市编码
		std::string Adcode = GetCityAdcode(City);
		// 提取时间
		
		// 获取指定时间地点的天气信息
	}
	return true;
}

bool FWeatherLogicAdapter::IsAskWeather(const std::string& Text,std::string& MatchText)
{
	// 询问天气得文本应该包含的词性顺序
	std::vector<std::string> AskWeatherTextTagVec = {
		"ns n",
		"ns uj n",
		"ns t uj n",
		"ns t m uj n",
		"ns t m m uj n"
		"ns l",
		"ns uj l",
		"ns t uj l",
		"ns t m uj l",
		"ns t m m uj l"
		"t ns uj l",
		"t ns uj n",
		"t m ns uj n",
		"t m ns uj l"
		"t m m ns uj n",
		"t m m ns uj l"
	};
	std::vector<std::pair<std::string,std::string>> Words;
	GlobalManager::jieba.Tag(Text,Words);
	std::string TextTag;
	std::string MatchTag;
	bool bTagFormatCorrect = false;
	bool bContaintWeather = false;
	// 提取文本的词性，并组合成对比格式
	for(std::pair<std::string,std::string> Pai : Words)
	{
		TextTag += Pai.second + " ";
	}
	// 判断询问文本词性字符串中是否包含标准天气询问词性格式子串
	for(std::string Tag : AskWeatherTextTagVec)
	{
		if(TextTag.find(Tag) != std::string::npos)
		{
			bTagFormatCorrect = true;
			MatchTag = Tag;
			break;
		}
	}
	const char* SplitChr = " ";
	std::vector<std::string> TagVec = GlobalManager::SplitString(MatchTag,SplitChr);
	int Index = 0;
	// 提取符合标准咨询天气信息词性字符串的询问文本子串
	for(std::pair<std::string,std::string> Pai : Words)
	{
		if(Pai.second == TagVec[Index])
		{
			MatchText += Pai.first;
			Index++;
		}
		else
		{
			MatchText = "";
			Index=0;
		}
	}
	Words.clear();
	GlobalManager::jieba.Tag(MatchText,Words);
	// 判断匹配子串中词性是名词或习用语的单词中是否包含“天气”一次，以进一步确认文本是否在询问天气信息
	for(int i = Words.size()-1;i>=0;--i)
	{
		if(Words[i].second == "n" || Words[i].second == "l")
		{
			if(Words[i].first.find("天气") != std::string::npos)
			{
				bContaintWeather = true;
				break;
			}
		}
	}
	return bTagFormatCorrect && bContaintWeather;
}

std::string FWeatherLogicAdapter::GetCityFromText(const std::string& Text)
{
	std::vector<std::pair<std::string,std::string>> Words;
	GlobalManager::jieba.Tag(Text,Words);
	std::string CityName;
	std::vector<int> IndexVec;
	for(std::pair<std::string,std::string> Word : Words)
	{
		if(Word.second == "ns")
		{
			CityName = Word.first;
			return CityName;
		}
	}
	UE_LOG(LOGNLP,Error,TEXT("There are no place names in the text %s"),*DebugLog::Log(Text));
	return "";
}

std::string FWeatherLogicAdapter::GetDateFromText(const std::string& Text)
{
	std::vector<std::pair<std::string,std::string>> Words;
	GlobalManager::jieba.Tag(Text,Words);
	std::string Date;
	// 连续标识，0为默认值，1表示连续，2表示中断
	int bContinuous = 0;
	for(std::pair<std::string,std::string> Pai : Words)
	{
		if((Pai.second == "t" || Pai.second == "m" || Pai.second == "i") && (bContinuous == 0 || bContinuous == 1))
		{
			Date += Pai.first;
			bContinuous = 1;
		}
		else
		{
			if(bContinuous == 1)
			{
				bContinuous = 2;
				break;
			}
		}
	}
	std::string NowDate;
	std::time_t Timet;
	std::time(&Timet);
	std::tm Tm;
	localtime_s(&Tm,&Timet);

	return "";
}

void FWeatherLogicAdapter::FormatDate(std::tm& Tm,std::string& Date)
{
	if(Date == "今天")
	{
		Date = std::to_string(Tm.tm_year) + "-" + std::to_string(Tm.tm_mon) + "-" + std::to_string(Tm.tm_mday);
	}
	else if(Date == "明天")
	{
		std::tm NowDate = GlobalManager::GetNowLocalTime();
		std::tm TimePeriod = {0};
		TimePeriod.tm_mday = 1;
		std::tm TargetDate = GlobalManager::TimeOperator(NowDate,TimePeriod);
		Date = GlobalManager::TmToString(TargetDate);
	}
	else if(Date == "后天")
	{
		std::tm NowDate = GlobalManager::GetNowLocalTime();
		std::tm TimePeriod = {0};
		TimePeriod.tm_mday = 2;
		std::tm TargetDate = GlobalManager::TimeOperator(NowDate,TimePeriod);
		Date = GlobalManager::TmToString(TargetDate);
	}
	else if(Date == "大后天")
	{
		std::tm NowDate = GlobalManager::GetNowLocalTime();
		std::tm TimePeriod = {0};
		TimePeriod.tm_mday = 3;
		std::tm TargetDate = GlobalManager::TimeOperator(NowDate,TimePeriod);
		Date = GlobalManager::TmToString(TargetDate);
	}
}

std::string FWeatherLogicAdapter::GetCityAdcode(const std::string& City)
{
	std::map<std::string,CityAdcode>::iterator It = CityAdcodeMap.find(City);
	if(It != CityAdcodeMap.end())
	{
		return It->second.Adcode;
	}
	else
	{
		UE_LOG(LOGNLP,Error,TEXT("City %s does not exist in the list"),*DebugLog::Log(City));
		return "";
	}
	
}

void FWeatherLogicAdapter::InitCityAdcode()
{
	std::ifstream Ifs;
	const std::string Path = GlobalManager::ResourcePath + CITYADCODEXLSX_PATH;
	Ifs.open(Path.c_str());
	check(Ifs.is_open());
	std::string Line;
	while (std::getline(Ifs,Line))
	{
		const char* SplitChr = "\t";
		std::vector<std::string> LineVec = GlobalManager::SplitString(Line,SplitChr);
		if(LineVec.size() == 3)
		{
			CityAdcode Code;
			std::string Name = LineVec[0];
			Code.Adcode = LineVec[1];
			Code.Citycode = LineVec[2];
			CityAdcodeMap.insert(std::pair<std::string,CityAdcode>(Name,Code));
		}
		else
		{
			UE_LOG(LOGNLP,Error,TEXT("The contents of the file %s are malformed"),*DebugLog::Log(Path));
		}
	}
	Ifs.close();
}
