#pragma once
#include "FLogicAdapter.h"
#include "CoreMinimal.h"

// 基础逻辑适配器，处理闲聊相关的问题
class NLP_API FBaseLogicAdapter : public FLogicAdapter
{
public:
	static FBaseLogicAdapter& Get();
	~FBaseLogicAdapter();
	
private:
	FBaseLogicAdapter();
	//FBaseLogicAdapter(const FBaseLogicAdapter& Single) = delete;
	//const FBaseLogicAdapter &operator=(const FBaseLogicAdapter& Single) = delete;
};
