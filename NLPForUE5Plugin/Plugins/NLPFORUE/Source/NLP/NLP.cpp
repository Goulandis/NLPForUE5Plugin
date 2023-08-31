#include "NLP.h"
#include "NLP/Common/LogDefine.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FNLP,NLP,"NLP");

DEFINE_LOG_CATEGORY(LOGNLP);

void FNLP::StartupModule()
{
	UE_LOG(LOGNLP,Log,TEXT("NLP module started up"));
	IModuleInterface::StartupModule();
}

void FNLP::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
