#include "FTimeLogicAdapter.h"
#include "NLP/Common/LogDefine.h"

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
