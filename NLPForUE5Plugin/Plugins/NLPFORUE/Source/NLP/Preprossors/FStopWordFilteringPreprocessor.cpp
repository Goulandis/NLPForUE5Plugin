#include "FStopWordFilteringPreprocessor.h"

FStopWordFilteringPreprocessor::FStopWordFilteringPreprocessor()
{
	NLOG(LOGNLP,Log,TEXT("StopWordFilteringPreprocessor constructed"));
}

FStopWordFilteringPreprocessor::~FStopWordFilteringPreprocessor()
{
	NLOG(LOGNLP,Log,TEXT("StopWordFilteringPreprocessor destructed"));
}

FStopWordFilteringPreprocessor& FStopWordFilteringPreprocessor::Get()
{
	static FStopWordFilteringPreprocessor Instance;
	return Instance;
}

std::string FStopWordFilteringPreprocessor::StopWordFiltering(string Text)
{
	vector<string> Words;
	std::string RelText;
	GlobalManager::jieba.Cut(Text,Words,true);
	for (size_t i = 0; i < Words.size(); ++i)
	{
		if (GlobalManager::jieba.extractor.GetStopWords().find(Words[i]) == GlobalManager::jieba.extractor.GetStopWords().end())
		{
			RelText += Words[i];
		}
	}
	return RelText;
}

std::string FStopWordFilteringPreprocessor::StopWordFiltering(vector<string> Words)
{
	std::string RelText;
	for (size_t i = 0; i < Words.size(); ++i)
	{
		if (GlobalManager::jieba.extractor.GetStopWords().find(Words[i]) == GlobalManager::jieba.extractor.GetStopWords().end())
		{
			RelText += Words[i];
		}
	}
	return RelText;
}


