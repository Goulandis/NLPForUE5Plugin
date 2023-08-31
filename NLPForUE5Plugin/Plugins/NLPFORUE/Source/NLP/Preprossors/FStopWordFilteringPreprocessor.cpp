#include "FStopWordFilteringPreprocessor.h"
#include "Define.h"
#include "NLP/Common/LogDefine.h"
#include "NLP/Common/GlobalManager.h"

FStopWordFilteringPreprocessor::FStopWordFilteringPreprocessor()
{
	UE_LOG(LOGNLP,Log,TEXT("StopWordFilteringPreprocessor constructed"));
}

FStopWordFilteringPreprocessor::~FStopWordFilteringPreprocessor()
{
	//UE_LOG(LOGNLP,Log,TEXT("StopWordFilteringPreprocessor destructed"));
}

FStopWordFilteringPreprocessor& FStopWordFilteringPreprocessor::CreateInstance()
{
	static FStopWordFilteringPreprocessor Instance;
	return Instance;
}

vector<string> FStopWordFilteringPreprocessor::StopWordFiltering(string Text)
{
	vector<string> words;
	vector<string> relwords;
	GlobalManager::jieba.Cut(Text,words,true);
	for (size_t i = 0; i < words.size(); ++i)
	{
		if (GlobalManager::jieba.extractor.GetStopWords().find(words[i]) == GlobalManager::jieba.extractor.GetStopWords().end())
		{
			relwords.push_back(words[i]);
		}
	}
	return relwords;
}

vector<string> FStopWordFilteringPreprocessor::StopWordFiltering(vector<string> Words)
{
	vector<string> relwords;
	for (size_t i = 0; i < Words.size(); ++i)
	{
		if (GlobalManager::jieba.extractor.GetStopWords().find(Words[i]) == GlobalManager::jieba.extractor.GetStopWords().end())
		{
			relwords.push_back(Words[i]);
		}
	}
	return relwords;
}


