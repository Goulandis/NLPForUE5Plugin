#include "PreprocessorFactory.h"
#include "FPreprossorsType.h"

DEFINE_LOG_CATEGORY(LOGNLP);

TSharedPtr<UPreprocessorFactory> UPreprocessorFactory::Instance = nullptr;

TSharedPtr<UPreprocessorFactory> UPreprocessorFactory::CreateInstance()
{
	if(!Instance.IsValid())
	{
		Instance = MakeShareable(NewObject<UPreprocessorFactory>());
	}
	return Instance;
}

UPreprocessorFactory::UPreprocessorFactory()
{
	UE_LOG(LOGNLP,Log,TEXT("PreprocessorFactory constructed"));
}

UPreprocessorFactory::~UPreprocessorFactory()
{
	UE_LOG(LOGNLP,Log,TEXT("PreprocessorFactory destructed"));
}

