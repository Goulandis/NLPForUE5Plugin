#pragma once
#include <unordered_set>
#include "FPreprocessor.h"
#include "CoreMinimal.h"

using namespace std;

class NLP_API FSensitiveWordPreprocessor : public FPreprocessor
{
public:
	static FSensitiveWordPreprocessor& CreateInstance();
	~FSensitiveWordPreprocessor();

	void LoadSensitiveWordDict(const string& Path);
	vector<string> SensitiveWordFiltering(string Text);
	//vector<string> SensitiveWordFiltering(vector<string> Words);
private:
	FSensitiveWordPreprocessor();

private:
	unordered_set<string> SensitiveWords;
};
