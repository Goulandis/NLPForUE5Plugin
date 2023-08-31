#pragma once
#include "FPreprocessor.h"
#include "CoreMinimal.h"

class NLP_API FStopWordFilteringPreprocessor : public FPreprocessor
{
public:
	static FStopWordFilteringPreprocessor& CreateInstance();
	
	~FStopWordFilteringPreprocessor();
	
	vector<string> StopWordFiltering(string Text);
	vector<string> StopWordFiltering(vector<string> Words);
private:
	FStopWordFilteringPreprocessor();
};
