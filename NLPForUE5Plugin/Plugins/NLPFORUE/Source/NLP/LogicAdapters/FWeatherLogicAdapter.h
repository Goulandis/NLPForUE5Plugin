#pragma once
#include "FLogicAdapter.h"
#include "CoreMinimal.h"

// 天气逻辑适配器，处理天气相关的问题
class NLP_API FWeatherLogicAdapter : public FLogicAdapter
{
public:
	static FWeatherLogicAdapter& CreateInstance();
	
	~FWeatherLogicAdapter();
private:
	FWeatherLogicAdapter();
};
