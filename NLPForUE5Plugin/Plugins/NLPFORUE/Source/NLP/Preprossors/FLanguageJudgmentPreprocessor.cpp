#include "FLanguageJudgmentPreprocessor.h"

FLanguageJudgmentPreprocessor::FLanguageJudgmentPreprocessor()
{
	NLOG(LOGNLP,Log,TEXT("LanguageJudgmentPreprocessor constructed"));
}

FLanguageJudgmentPreprocessor::~FLanguageJudgmentPreprocessor()
{
	NLOG(LOGNLP,Log,TEXT("LanguageJudgmentPreprocessor destructed"));
}

FLanguageJudgmentPreprocessor& FLanguageJudgmentPreprocessor::CreateInstance()
{
	static FLanguageJudgmentPreprocessor Instance;
	return Instance;
}

GlobalManager::ELanguageType FLanguageJudgmentPreprocessor::GetLanguageType(std::string& Text)
{
	// 只要字符串中包含一个中文字符则认为字符串为中文
	if(GlobalManager::IsChinese(Text))
	{
		return GlobalManager::ELanguageType::zh_CN;
	}
	else
	{
		// 如果字符串不包含中文字符，但匹配数学算式，默认抛到中文处理流程中处理
		if(GlobalManager::RegexMathFormulas(Text))
		{
			return GlobalManager::ELanguageType::zh_CN;
		}
		// 如果字符串不符合上面所有判断，则认为字符串为英文，目前只处理中文和英文
		else
		{
			return GlobalManager::ELanguageType::en_US;
		}
	}
}
