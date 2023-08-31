#pragma once
#include "FLogicAdapter.h"
#include "CoreMinimal.h"

// 基础逻辑适配器，处理闲聊相关的问题
class NLP_API FBaseLogicAdapter : public FLogicAdapter
{
public:
	static FBaseLogicAdapter& CreateInstance();
	
	~FBaseLogicAdapter();
private:
	FBaseLogicAdapter();
};
