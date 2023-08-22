#include "FSensitiveWordPreprocessor.h"
#include "FPreprossorsType.h"
#include <fstream>
#include <assert.h>

FSensitiveWordPreprocessor::FSensitiveWordPreprocessor()
{
	UE_LOG(LOGNLP,Log,TEXT("FSensitiveWordPreprocessor constructed"));
}

FSensitiveWordPreprocessor& FSensitiveWordPreprocessor::CreateInstance()
{
	static FSensitiveWordPreprocessor Instance;
	return Instance;
}

FSensitiveWordPreprocessor::~FSensitiveWordPreprocessor()
{
	UE_LOG(LOGNLP,Log,TEXT("FSensitiveWordPreprocessor destructed"));
}

void FSensitiveWordPreprocessor::LoadSensitiveWordDict(const string& Path)
{
	ifstream Ifs(Path.c_str());
	check(Ifs.is_open())
	string Line;
	while(getline(Ifs,Line))
	{
		SensitiveWords.insert(Line);
	}
	if(SensitiveWords.size() == 0)
	{
		UE_LOG(LOGNLP,Error,TEXT("There is no set dictionary of sensitive words"));
	}
}

vector<string> FSensitiveWordPreprocessor::SensitiveWordFiltering(string Text)
{
	
}

// vector<string> FSensitiveWordPreprocessor::SensitiveWordFiltering(vector<string> Words)
// {
// 	
// }
