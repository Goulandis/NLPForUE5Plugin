#include "FWeatherLogicAdapter.h"
#include "NLP/Common/LogDefine.h"

FWeatherLogicAdapter::FWeatherLogicAdapter()
{
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
