#include "FLogicAdapterModule.h"
#include "NLP/Common/LogDefine.h"

FLogicAdapterModule::FLogicAdapterModule()
{
	UE_LOG(LOGNLP,Log,TEXT("FLogicAdapterModule constructed"));
}

FLogicAdapterModule::~FLogicAdapterModule()
{
	UE_LOG(LOGNLP,Log,TEXT("FLogicAdapterModule destructed"));
}
