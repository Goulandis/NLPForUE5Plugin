#include "FPreprocessorModule.h"
#include "NLP/Common/LogDefine.h"

FPreprocessorModule::FPreprocessorModule()
{
	FString SensitiveWordDictPath = FPaths::ProjectPluginsDir() + SENSITIVE_WORD_PATH;
	string SensitiveWordDictPathCStr = TCHAR_TO_UTF8(*SensitiveWordDictPath);
	UE_LOG(LOGNLP,Log,TEXT("Loading sensitive word from %s"),*SensitiveWordDictPath);
	FPreprocessorFactory::CreateInstance()->GetPreprocessor<FSensitiveWordPreprocessor>()->LoadSensitiveWordDict(SensitiveWordDictPathCStr);
	UE_LOG(LOGNLP,Log,TEXT("Loaded sensitive word from %s"),*SensitiveWordDictPath);
	UE_LOG(LOGNLP,Log,TEXT("FPreprocessorModule constructed"));
}

FPreprocessorModule::~FPreprocessorModule()
{
	UE_LOG(LOGNLP,Log,TEXT("FPreprocessorModule destructed"));
}

TArray<FString> FPreprocessorModule::Handle(FString& Text) const
{
	string TextLocal = TCHAR_TO_UTF8(*Text);
	string RelTmp;
	vector<string> RelVec;
	TArray<FString> RelTArr;
	ELanguageType LType = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FLanguageJudgmentPreprocessor>()->GetLanguageType(TextLocal);
	switch (LType)
	{
	case ELanguageType::zh_CN:
		RelTmp = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FSensitiveWordPreprocessor>()->SensitiveWordFiltering(TextLocal);
		RelTmp = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FSpecialSymbolPreprocessor>()->DeteleSpecialSymbol(RelTmp);
		RelVec = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FStopWordFilteringPreprocessor>()->StopWordFiltering(RelTmp);
		break;;
	case ELanguageType::zh_HK:
		break;
	case ELanguageType::en_US:
		break;
	default:
		break;
	}
	for(string Str : RelVec)
	{
		RelTArr.Add(UTF8_TO_TCHAR(Str.c_str()));
	}
	return RelTArr;
}
