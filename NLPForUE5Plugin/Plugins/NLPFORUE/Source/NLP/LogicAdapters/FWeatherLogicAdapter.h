#pragma once
#include "FLogicAdapter.h"
#include "CoreMinimal.h"
//#include <map>


struct CityAdcode
{
	std::string Adcode;
	std::string Citycode;
};

// 天气逻辑适配器，处理天气相关的问题
class NLP_API FWeatherLogicAdapter : public FLogicAdapter
{
public:
	static FWeatherLogicAdapter& Get();

	bool Process(const std::string& Input,std::string& Output);
	// 判断一个句子是否是在询问天气信息
	bool IsAskWeather(const std::string& Text,std::string& MatchText);
	// 从一句文本中提取地名，只提取第一个获取到的地名
	std::string GetCityFromText(const std::string& Text);
	// 从文本中提取日期信息
	void GetDateFromText(std::tm& Tm,const std::string& Text);
	void GetDateFromText2(std::tm& Tm,const std::string& Text);
	// 从获取天气信息
	std::string GetWeatherInfo(const std::string& City,const std::tm& Date);
	// 根据获取到天气信息生成相应的回答
	std::string SpawnWeatherLogicAdapterAnswer(std::string& JsonStr,const std::string& City);
	// 从城市编码列表中寻找指定名称的城市编码
	std::string GetCityAdcode(const std::string& City);
	bool IsRightDate(const std::tm Date);
	
private:
	FWeatherLogicAdapter();
	~FWeatherLogicAdapter();
	FWeatherLogicAdapter(const FWeatherLogicAdapter& Single) = delete;
	
	void InitCityAdcode();
	void InitAskWeatherTextTag();

	// 存储城市编码的map
	std::map<std::string,CityAdcode> CityAdcodeMap;
	// 询问天气的文本应该包含的词性顺序vector
	std::vector<std::string> AskWeatherTextTagVec;
};
