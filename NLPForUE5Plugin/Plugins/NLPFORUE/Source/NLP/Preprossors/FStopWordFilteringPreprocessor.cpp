#include "FStopWordFilteringPreprocessor.h"
#include "FPreprossorsType.h"

FStopWordFilteringPreprocessor::FStopWordFilteringPreprocessor()
{
	Tokenizer = new cppjieba::Jieba(
		TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + DICT_PATH)),
		TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + HMM_PATH)),
		TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + USER_DICT_PATH)),
		TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + IDF_PATH)),
		TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + STOP_WORD_PATH))
		);
	UE_LOG(LOGNLP,Log,TEXT("StopWordFilteringPreprocessor constructed"));
}

FStopWordFilteringPreprocessor::~FStopWordFilteringPreprocessor()
{
	UE_LOG(LOGNLP,Log,TEXT("StopWordFilteringPreprocessor destructed"));
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
	Tokenizer->Cut(Text,words,true);
	for (size_t i = 0; i < words.size(); ++i)
	{
		if (Tokenizer->extractor.GetStopWords().find(words[i]) == Tokenizer->extractor.GetStopWords().end())
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
		if (Tokenizer->extractor.GetStopWords().find(Words[i]) == Tokenizer->extractor.GetStopWords().end())
		{
			relwords.push_back(Words[i]);
		}
	}
	return relwords;
}


