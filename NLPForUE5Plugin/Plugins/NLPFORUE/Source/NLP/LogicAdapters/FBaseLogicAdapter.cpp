#include "FBaseLogicAdapter.h"

FBaseLogicAdapter::FBaseLogicAdapter()
{
	NLOG(LOGNLP,Log,TEXT("FBaseLogicAdapter constructed"));
}

FBaseLogicAdapter& FBaseLogicAdapter::Get()
{
	static FBaseLogicAdapter Instance;
	return Instance;
}

FBaseLogicAdapter::~FBaseLogicAdapter()
{
	NLOG(LOGNLP,Log,TEXT("FBaseLogicAdapter destructed"));
}
