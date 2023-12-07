#include "FPreprocessorModule.h"
#include "NLP/Common/GlobalManager.h"

FPreprocessorModule::FPreprocessorModule()
{
	std::string SensitiveWordPath = ConfigManager::CreateInstance().SensitiveWordPreprocessorConfig.at("SensitiveWordPath");
	const std::string SensitiveWordDictPath = GlobalManager::RESOURCE_ABSOLUTE_PATH + SensitiveWordPath;
	NLOG(LOGNLP,Log,TEXT("Loading sensitive word from %s"),*TOFSTR(SensitiveWordDictPath));
	FPreprocessorFactory::CreateInstance()->GetPreprocessor<FSensitiveWordPreprocessor>()->LoadSensitiveWordDict(SensitiveWordDictPath);
	NLOG(LOGNLP,Log,TEXT("Loaded sensitive word from %s"),*TOFSTR(SensitiveWordDictPath));
	NLOG(LOGNLP,Log,TEXT("FPreprocessorModule constructed"));
}

FPreprocessorModule::~FPreprocessorModule()
{
	NLOG(LOGNLP,Log,TEXT("FPreprocessorModule destructed"));
}

TArray<FString> FPreprocessorModule::Handle(FString& Text) const
{
	string TextLocal = TCHAR_TO_UTF8(*Text);
	string RelTmp;
	vector<string> RelVec;
	TArray<FString> RelTArr;
	GlobalManager::ELanguageType LType = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FLanguageJudgmentPreprocessor>()->GetLanguageType(TextLocal);
	switch (LType)
	{
	case GlobalManager::ELanguageType::zh_CN:
		RelTmp = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FSensitiveWordPreprocessor>()->SensitiveWordFiltering(TextLocal);
		RelTmp = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FSpecialSymbolPreprocessor>()->DeteleSpecialSymbol(RelTmp);
		RelVec = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FStopWordFilteringPreprocessor>()->StopWordFiltering(RelTmp);
		break;;
	case GlobalManager::ELanguageType::zh_HK:
		break;
	case GlobalManager::ELanguageType::en_US:
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
