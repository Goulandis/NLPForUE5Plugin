#include "NLP.h"
#include "Preprossors/FPreprossorsType.h"

//DEFINE_LOG_CATEGORY(LOGNLP);

IMPLEMENT_PRIMARY_GAME_MODULE(FNLP,NLP,"NLP");

void FNLP::StartupModule()
{
	UE_LOG(LOGNLP,Log,TEXT("NLP module started up"));
	//IModuleInterface::StartupModule();
}

void FNLP::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
