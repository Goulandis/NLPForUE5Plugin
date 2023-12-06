#include "FWeatherLogicAdapter.h"
#include <fstream>
#include "cpp-httplib/httplib.h"

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
		// 提取时间
		std::tm Date = {0};
		GetDateFromText(Date,MatchText);
		// 获取指定时间地点的天气信息
		std::string WeatherInfo = GetWeatherInfo(City,Date);
		Output = SpawnWeatherLogicAdapterAnswer(WeatherInfo,City);
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
		"ns t m m uj n",
		"ns x m m m uj n",
		"ns m m uj n ",
		"ns m m m m x m uj n",
		"ns i uj n",
		"ns l",
		"ns uj l",
		"ns t uj l",
		"ns t m uj l",
		"ns t m m uj l",
		"t ns uj l",
		"t ns uj n",
		"t m ns uj n",
		"t m ns uj l",
		"t m m ns uj n",
		"t m m ns uj l",
		"nz m m uj n ",
		"nz n",
		"nz uj n",
		"nz t uj n",
		"nz t m uj n",
		"nz t m m uj n",
		"nz x m m m uj n",
		"nz m m m m x m uj n",
		"nz i uj n",
		"nz l",
		"nz uj l",
		"nz t uj l",
		"nz t m uj l",
		"nz t m m uj l",
		"t nz uj l",
		"t nz uj n",
		"t m nz uj n",
		"t m nz uj l",
		"t m m nz uj n",
		"t m m nz uj l",
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
	if(MatchTag.empty())
	{
		UE_LOG(LOGNLP,Log,TEXT("没有匹配的天气询问词性组合，当前文本词性:%s"),*TOFSTR(TextTag));
		return false;
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
	// 判断匹配子串中词性是名词或习用语的单词中是否包含“天气”一词，以进一步确认文本是否在询问天气信息
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
		if(Word.second == "ns" || Word.second == "nz")
		{
			std::map<std::string,CityAdcode>::iterator It = CityAdcodeMap.find(Word.first);
			// 在城市列表中寻找包含关键词的地名
			for(std::pair<std::string,CityAdcode> Pai : CityAdcodeMap)
			{
				if(Pai.first.find(Word.first) != std::string::npos)
				{
					CityName = Pai.first;
					return CityName;
				}
			}
		}
	}
	UE_LOG(LOGNLP,Error,TEXT("There are no place names in the text %s"),*DebugLog::Log(Text));
	return CityName;
}

void FWeatherLogicAdapter::GetDateFromText(std::tm& Tm,std::string& Text)
{
	std::regex PattrenT,PattrenNum,PattrenCN;
	std::smatch MatchsT,MatchsNum,MatchsCN;
	std::string PrefixNum = R"(\d+年\b(?:[1-9]|1[0-2])\b月\b(?:[1-9]|1\d|2[0-9]|3[0-1])\b)";
	std::string PrefixCN = R"([零一二三四五六七八九]+年[零一二三四五六七八九十]+月[零一二三四五六七八九十]+)";
	PattrenNum = PrefixNum + "日|" + PrefixNum + "号|" + PrefixNum;
	PattrenCN = PrefixCN + "日|" + PrefixCN + "号|" + PrefixCN;
	PattrenT = R"(今天|明天|后天|大后天)";
	// 如果日期是时间代词
	if(std::regex_search(Text,MatchsT,PattrenT) && MatchsT.size() > 0)
	{
		std::tm TimePeriod = {0};
		if(MatchsT[0] == "今天")
		{
			Tm = GlobalManager::GetNowLocalTime();
		}
		else if(MatchsT[0] == "明天")
		{
			Tm = GlobalManager::GetNowLocalTime();
			UE_LOG(LOGNLP,Log,TEXT("Tm1:%s"),*TOFSTR(GlobalManager::TmToString(Tm)));
			TimePeriod.tm_mday = 1;
			UE_LOG(LOGNLP,Log,TEXT("TimePeriod:%s"),*TOFSTR(GlobalManager::TmToString(TimePeriod)));
			Tm = GlobalManager::TimeOperator(Tm,TimePeriod);
			UE_LOG(LOGNLP,Log,TEXT("Tm2:%s"),*TOFSTR(GlobalManager::TmToString(Tm)));
		}
		else if(MatchsT[0] == "后天")
		{
			Tm = GlobalManager::GetNowLocalTime();
			TimePeriod.tm_mday = 2;
			Tm = GlobalManager::TimeOperator(Tm,TimePeriod);
		}
		else if(MatchsT[0] == "大后天")
		{
			Tm = GlobalManager::GetNowLocalTime();
			TimePeriod.tm_mday = 3;
			Tm = GlobalManager::TimeOperator(Tm,TimePeriod);
		}
		
	}
	// 如果日期是阿拉伯数字格形式，如：2023年11月21日
	else if(std::regex_search(Text,MatchsNum,PattrenNum))
	{
		std::vector<std::string> Words;
		GlobalManager::jieba.CutHMM(Text,Words);
		if(Words.size() == 6)
		{
			Tm.tm_year = GlobalManager::IsNumber(Words[0])?std::stoi(Words[0]):0;
			Tm.tm_mon = GlobalManager::IsNumber(Words[2])?std::stoi(Words[2]):0;
			Tm.tm_mday = GlobalManager::IsNumber(Words[4])?std::stoi(Words[4]):0;
		}
		else
		{
			UE_LOG(LOGNLP,Error,TEXT("Date format error : %s  Words.size=%d"),*TOFSTR(Text),(int)Words.size());
		}
	}
	// 如果日期是中文数字形式，如：二零二三年十一月二十一日
	else if(std::regex_search(Text,MatchsCN,PattrenCN))
	{
		std::vector<std::string> Words = GlobalManager::SplitTextToWord(Text);
		std::vector<std::string> NumWords;
		std::string NumWord;
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
		else
		{
			UE_LOG(LOGNLP,Error,TEXT("Date format error : %s NumWords.size=%d"),*TOFSTR(Text),(int)NumWords.size());
		}
	}
	// 如果没有明确的时间提示词，则去当前日期
	else
	{
		Tm = GlobalManager::GetNowLocalTime();
	}
}

std::string FWeatherLogicAdapter::GetWeatherInfo(const std::string& City, const std::tm& Date)
{
	// 获取城市编码
	std::string Adcode = GetCityAdcode(City);
	// 向高德平台发起https的get请求，查询指定城市今天、明天、后天和大后天的天气信息
	httplib::Client HttpClient("restapi.amap.com");
	httplib::Params Params;
	Params.emplace("key","62aac7f2fbe5a09d9c69f611d238034b");
	Params.emplace("city",Adcode);
	Params.emplace("extensions","all");
	httplib::Headers Heard = {{"Content-Type","application/x-www-from-urlencoded"}};
	httplib::Result Res = HttpClient.Get("/v3/weather/weatherInfo",Params,Heard);
	// 解析从高德平台过去到的天气信息Json字符串
	std::string JsonStr;
	nlohmann::json Root = nlohmann::json::parse(Res->body);
	if(Root.is_null())
	{
		UE_LOG(LOGNLP,Error,TEXT("Failed to cast json string to nlohmann::json"));
		return JsonStr;
	}
	nlohmann::json Forecasts = Root.at("forecasts");
	if(Forecasts.is_array() && Forecasts.size() > 0)
	{
		nlohmann::json Casts;
		// 寻找匹配城市的天气信息(高德支持同时查询多个城市的天气信息)
		for(int i=0;i<Forecasts.size();++i)
		{
			std::string FCity = Forecasts[i].at("city");
			if(City == Forecasts[i].at("city"))
			{
				Casts = Forecasts[i].at("casts");
				break;
			}
		}
		if(Casts.is_array() && Casts.size() > 0)
		{
			for(int i=0;i<Casts.size();++i)
			{
				if(!Casts[i].is_object())
				{
					break;
				}
				// 获取指定日期的天气信息
				std::tm Tm = GlobalManager::StringToTm(Casts[i].at("date"));
				if (Tm.tm_year == Date.tm_year && Tm.tm_mon == Date.tm_mon && Tm.tm_mday == Date.tm_mday)
				{
					JsonStr = Casts[i].dump();
				}
			}
		}
	}
	if(JsonStr.empty())
	{
		UE_LOG(LOGNLP,Error,TEXT("Json value is null,Json:%s"),*TOFSTR(Res->body));
	}
	return JsonStr;
}

std::string FWeatherLogicAdapter::SpawnWeatherLogicAdapterAnswer(std::string& JsonStr, const std::string& City)
{
	if(JsonStr.empty())
	{
		return "没有查询到天气信息";
	}
	std::stringstream Fmt;
	nlohmann::json Root = nlohmann::json::parse(JsonStr);
	Fmt<<City<<Root.at("date")<<"的天气情况：\n"<<"白天：\n";
	Fmt<<"天气："<<Root.at("dayweather")<<"，温度："<<Root.at("daytemp")<<"，风向："<<Root.at("daywind")<<"，风力："<<Root.at("daypower")<<"\n";
	Fmt<<"夜间：\n天气："<<Root.at("nightweather")<<"，温度："<<Root.at("nighttemp")<<"，风向："<<Root.at("nightwind")<<"，风力："<<Root.at("nightpower")<<"\n";
	return Fmt.str();
}

std::string FWeatherLogicAdapter::GetCityAdcode(const std::string& City)
{
	std::map<std::string,CityAdcode>::iterator It = CityAdcodeMap.find(City);
	// 在城市列表中寻找包含关键词的地名
	for(std::pair<std::string,CityAdcode> Pai : CityAdcodeMap)
	{
		if(Pai.first.find(City) != std::string::npos)
		{
			return Pai.second.Adcode;
		}
	}
	UE_LOG(LOGNLP,Error,TEXT("City %s does not exist in the list"),*DebugLog::Log(City));
	return "";
}

void FWeatherLogicAdapter::InitCityAdcode()
{
	std::ifstream Ifs;
	const std::string Path = GlobalManager::RESOURCE_PATH + GlobalManager::CITYADCODEXLSX_PATH;
	Ifs.open(Path.c_str());
	check(Ifs.is_open());
	std::string Line;
	while (std::getline(Ifs,Line))
	{
		const char* SplitChr = "\t";
		std::vector<std::string> LineVec = GlobalManager::SplitString(Line,SplitChr);
		if(LineVec.size() >= 2)
		{
			CityAdcode Code;
			std::string Name;
			if(LineVec.size() == 3)
			{
				Name = LineVec[0];
				Code.Adcode = LineVec[1];
				Code.Citycode = LineVec[2];
			}
			else if(LineVec.size() == 2)
			{
				Name = LineVec[0];
				Code.Adcode = LineVec[1];
				Code.Citycode = -1;
			}
			CityAdcodeMap.insert(std::pair<std::string,CityAdcode>(Name,Code));
		}
		else
		{
			UE_LOG(LOGNLP,Error,TEXT("The contents of the file %s are malformed"),*DebugLog::Log(Path));
		}
	}
	Ifs.close();
}