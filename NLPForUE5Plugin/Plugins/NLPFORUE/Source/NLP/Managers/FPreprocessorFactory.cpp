#include "FPreprocessorFactory.h"
#include "../Preprossors/FPreprossorsType.h"

DEFINE_LOG_CATEGORY(LOGNLP);

FPreprocessorFactory* FPreprocessorFactory::Instance = nullptr;

FPreprocessorFactory::FPreprocessorFactory()
{
	UE_LOG(LOGNLP,Log,TEXT("FPreprocessorFactory constructed"));
}

FPreprocessorFactory::~FPreprocessorFactory()
{
	UE_LOG(LOGNLP,Log,TEXT("FPreprocessorFactory destructed"));
}

FPreprocessorFactory* FPreprocessorFactory::CreateInstance()
{
	if(!Instance)
	{
		Instance = new FPreprocessorFactory();
		Instance->LanguageJudgmentPreprocessor = &GetPreprocessorSelf<FLanguageJudgmentPreprocessor>();
		Instance->SensitiveWordPreprocessor = &GetPreprocessorSelf<FSensitiveWordPreprocessor>();
		Instance->SpecialSymbolPreprocessor = &GetPreprocessorSelf<FSpecialSymbolPreprocessor>();
		Instance->StopWordFilteringPreprocessor = &GetPreprocessorSelf<FStopWordFilteringPreprocessor>();
	}
	return Instance;
}
