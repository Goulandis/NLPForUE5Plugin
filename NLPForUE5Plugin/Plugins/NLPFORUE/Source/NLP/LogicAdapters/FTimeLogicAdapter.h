#pragma once
#include "FLogicAdapter.h"
#include "CoreMinimal.h"

//时间逻辑适配器，处理时间相关的问题
class NLP_API FTimeLogicAdapter : public FLogicAdapter
{
public:
	enum EAskType { Date,Week,Time };
	static FTimeLogicAdapter& Get();
	~FTimeLogicAdapter();

	bool Process(const std::string& Input,std::string& Output);
	bool IsAskTime(const std::string& Text,std::string& MatchStr,EAskType& AskType);
	std::tm GetTimeFromText(const std::string& Text);
	std::string SpawnAnswer(const std::tm& Time,const EAskType AskType);
private:
	FTimeLogicAdapter();
	
	//FTimeLogicAdapter(const FTimeLogicAdapter& Single) = delete;
	//const FTimeLogicAdapter &operator=(const FTimeLogicAdapter& Single) = delete;
};
