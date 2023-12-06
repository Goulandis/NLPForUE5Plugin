#include "FTimeLogicAdapter.h"
#include "NLP/Common/GlobalManager.h"

FTimeLogicAdapter::FTimeLogicAdapter()
{
	UE_LOG(LOGNLP,Log,TEXT("FTimeLogicAdapter constructed"));
}

FTimeLogicAdapter& FTimeLogicAdapter::CreateInstance()
{
	static FTimeLogicAdapter Instance;
	return Instance;
}

FTimeLogicAdapter::~FTimeLogicAdapter()
{
	UE_LOG(LOGNLP,Log,TEXT("FTimeLogicAdapter destructed"));
}
