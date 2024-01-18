#pragma once
#include "FPreprocessor.h"
#include "CoreMinimal.h"

class NLP_API FStopWordFilteringPreprocessor : public FPreprocessor
{
public:
	static FStopWordFilteringPreprocessor& Get();
	~FStopWordFilteringPreprocessor();
	
	std::string StopWordFiltering(string Text);
	std::string StopWordFiltering(vector<string> Words);
private:
	FStopWordFilteringPreprocessor();
	
};
