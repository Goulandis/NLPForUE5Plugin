#include "FLogicAdapterModule.h"
#include "NLP/Common/GlobalManager.h"

FLogicAdapterModule::FLogicAdapterModule()
{
	NLOG(LOGNLP,Log,TEXT("FLogicAdapterModule constructed"));
}

FLogicAdapterModule::~FLogicAdapterModule()
{
	NLOG(LOGNLP,Log,TEXT("FLogicAdapterModule destructed"));
}
