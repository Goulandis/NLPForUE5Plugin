#include "FTimeLogicAdapter.h"

FTimeLogicAdapter::FTimeLogicAdapter()
{
	NLOG(LOGNLP,Log,TEXT("FTimeLogicAdapter constructed"));
}

FTimeLogicAdapter& FTimeLogicAdapter::CreateInstance()
{
	static FTimeLogicAdapter Instance;
	return Instance;
}

FTimeLogicAdapter::~FTimeLogicAdapter()
{
	NLOG(LOGNLP,Log,TEXT("FTimeLogicAdapter destructed"));
}
