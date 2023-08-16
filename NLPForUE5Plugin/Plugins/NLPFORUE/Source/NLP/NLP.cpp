#include "NLP.h"
#include "../NLPFORUE/Common/FDefine.h"

DEFINE_LOG_CATEGORY(LOGNLPFORUE);

IMPLEMENT_PRIMARY_GAME_MODULE(FNLP,NLP,"NLP");

void FNLP::StartupModule()
{
	UE_LOG(LOGNLPFORUE,Log,TEXT("NLP module started up"));
	//IModuleInterface::StartupModule();
}

void FNLP::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
