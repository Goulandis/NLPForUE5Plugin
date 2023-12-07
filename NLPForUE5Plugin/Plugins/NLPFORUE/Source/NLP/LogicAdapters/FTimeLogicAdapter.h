#pragma once
#include "FLogicAdapter.h"
#include "CoreMinimal.h"

//时间逻辑适配器，处理时间相关的问题
class NLP_API FTimeLogicAdapter : public FLogicAdapter
{
public:
	static FTimeLogicAdapter& CreateInstance();
	
	~FTimeLogicAdapter();

	bool Process(const std::string& Input,const std::string& Output);
	bool IsAskTime(const std::string& Text);

	std::wstring StringToWideString(const std::string& Str);
private:
	FTimeLogicAdapter();
};
