#pragma once
#include "NLP/LogicAdapters/FBaseLogicAdapter.h"
#include "NLP/LogicAdapters/FMathLogicAdapter.h"
#include "NLP/LogicAdapters/FTimeLogicAdapter.h"
#include "NLP/LogicAdapters/FWeatherLogicAdapter.h"
#include "CoreMinimal.h"

class NLP_API FLogicAdapterModule
{
public:
	struct SConfig
	{
		SConfig(bool m = true,bool w = true,bool t = true,bool b = true):
			bMath(m),bWeather(w),bTime(t),bBase(b){}
		bool bMath = true;
		bool bWeather = true;
		bool bTime = true;
		bool bBase = true;
	};
	FLogicAdapterModule();
	~FLogicAdapterModule();

	void Handle(const std::string& Input,std::string& Output,const SConfig& InConfig=SConfig());
	void ModuleConfig(const SConfig& InConfig);

	FMathLogicAdapter* Loap_Math;
	FWeatherLogicAdapter* Loap_Weather;
	FTimeLogicAdapter* Loap_Time;
	FBaseLogicAdapter* Loap_Base;
private:
	template<typename T>
	static T& GetLogicAdapter()
	{
		return T::Get();
	}
	
	SConfig Config;
};
