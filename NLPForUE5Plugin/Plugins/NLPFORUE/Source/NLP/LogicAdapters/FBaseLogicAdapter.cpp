#include "FBaseLogicAdapter.h"
#include "NLP/Common/LogDefine.h"

FBaseLogicAdapter::FBaseLogicAdapter()
{
	UE_LOG(LOGNLP,Log,TEXT("FBaseLogicAdapter constructed"));
}

FBaseLogicAdapter& FBaseLogicAdapter::CreateInstance()
{
	static FBaseLogicAdapter Instance;
	return Instance;
}

FBaseLogicAdapter::~FBaseLogicAdapter()
{
	UE_LOG(LOGNLP,Log,TEXT("FBaseLogicAdapter destructed"));
}
