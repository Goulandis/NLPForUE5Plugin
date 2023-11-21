#pragma once
#include "FLogicAdapter.h"
#include "CoreMinimal.h"
#include "NLP/Common/LogDefine.h"
#include <map>

struct CityAdcode
{
	std::string Adcode;
	std::string Citycode;
};

// 天气逻辑适配器，处理天气相关的问题
class NLP_API FWeatherLogicAdapter : public FLogicAdapter
{
public:
	static FWeatherLogicAdapter& CreateInstance();
	
	~FWeatherLogicAdapter();

	bool Process(const std::string& Input,std::string& Output);
	// 判断一个句子是否是在询问天气信息
	bool IsAskWeather(const std::string& Text,std::string& MatchText);
	// 从一句文本中提取地名，只提取第一个获取到的地名
	std::string GetCityFromText(const std::string& Text);
	std::string GetDateFromText(const std::string& Text);
	void FormatDate(std::tm& Tm,std::string& Date);
	// 从获取天气信息
	std::string GetWeatherInfo(const std::string& City,const std::string& Time);
	// 从城市编码列表中寻找指定名称的城市编码
	std::string GetCityAdcode(const std::string& City);
private:
	FWeatherLogicAdapter();
	void InitCityAdcode();

	// 存储城市编码的map
	std::map<std::string,CityAdcode> CityAdcodeMap;
};
