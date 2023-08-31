#pragma once
#include "FLogicAdapter.h"
#include "CoreMinimal.h"

//时间逻辑适配器，处理时间相关的问题
class NLP_API FTimeLogicAdapter : public FLogicAdapter
{
public:
	static FTimeLogicAdapter& CreateInstance();
	
	~FTimeLogicAdapter();
private:
	FTimeLogicAdapter();
};
