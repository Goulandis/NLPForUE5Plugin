#include "FPreprocessorFactory.h"
#include "NLP/Common/GlobalManager.h"

FPreprocessorFactory* FPreprocessorFactory::Instance = nullptr;

// void FPreprocessorFactory::DestroyInstance()
// {
// 	if(Instance->LanguageJudgmentPreprocessor)
// 	{
// 		delete Instance->LanguageJudgmentPreprocessor;
// 		Instance->LanguageJudgmentPreprocessor = nullptr;
// 	}
// 	if(Instance->SensitiveWordPreprocessor)
// 	{
// 		delete Instance->SensitiveWordPreprocessor;
// 		Instance->SensitiveWordPreprocessor = nullptr;
// 	}
// 	if(Instance->SpecialSymbolPreprocessor)
// 	{
// 		delete Instance->SpecialSymbolPreprocessor;
// 		Instance->SpecialSymbolPreprocessor = nullptr;
// 	}
// 	if(Instance->StopWordFilteringPreprocessor)
// 	{
// 		delete Instance->StopWordFilteringPreprocessor;
// 		Instance->StopWordFilteringPreprocessor = nullptr;
// 	}
// 	// if(Instance)
// 	// {
// 	// 	delete Instance;
// 	// 	Instance = nullptr;
// 	// }
// }

FPreprocessorFactory::FPreprocessorFactory()
{
	NLOG(LOGNLP,Log,TEXT("FPreprocessorFactory constructed"));
}

FPreprocessorFactory::~FPreprocessorFactory()
{
	NLOG(LOGNLP,Log,TEXT("FPreprocessorFactory destructed"));
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
