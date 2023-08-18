#include "FLanguageJudgmentPreprocessor.h"

DEFINE_LOG_CATEGORY(LOGNLP);

FLanguageJudgmentPreprocessor::FLanguageJudgmentPreprocessor()
{
	UE_LOG(LOGNLP,Log,TEXT("LanguageJudgmentPreprocessor constructed"));
}

FLanguageJudgmentPreprocessor::~FLanguageJudgmentPreprocessor()
{
	UE_LOG(LOGNLP,Log,TEXT("LanguageJudgmentPreprocessor destructed"));
}

FLanguageJudgmentPreprocessor& FLanguageJudgmentPreprocessor::CreateInstance()
{
	static FLanguageJudgmentPreprocessor Instance;
	return Instance;
}

ELanguageType FLanguageJudgmentPreprocessor::GetLanguageType(std::string& Text)
{
	int Len = Text.size()+1;
	char* TChar = new char[Len];
	strcpy_s(TChar,Len,Text.c_str());
	char Chr;
	while(true)
	{
		Chr = *TChar++;
		// 如果字符串直到结尾都没有出现中文字符，则认为字符串为英文
		if(Chr == 0)
		{
			break;
		}
		// 如果字符串中出现了中文，则认为字符串为中文
		if((Chr & 0x80) && (*TChar & 0x80))
		{
			delete TChar;
			return ELanguageType::zh_CN;
		}
	}
	delete TChar;
	return ELanguageType::en_US;
	
}

ELanguageType FLanguageJudgmentPreprocessor::GetLanguageType(FString Text)
{
	std::string Str = TCHAR_TO_UTF8(*Text);
	return GetLanguageType(Str);
}
