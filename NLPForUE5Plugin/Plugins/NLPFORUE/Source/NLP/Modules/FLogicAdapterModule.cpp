#include "FLogicAdapterModule.h"

#include "Chaos/ImplicitQRSVD.h"
#include "NLP/Common/GlobalManager.h"

FLogicAdapterModule::FLogicAdapterModule()
{
	Loap_Math = FLogicAdapterFactory::CreateInstance()->GetLogicAdapter<FMathLogicAdapter>();
	Loap_Weather = FLogicAdapterFactory::CreateInstance()->GetLogicAdapter<FWeatherLogicAdapter>();
	Loap_Time = FLogicAdapterFactory::CreateInstance()->GetLogicAdapter<FTimeLogicAdapter>();
	Loap_Base = FLogicAdapterFactory::CreateInstance()->GetLogicAdapter<FBaseLogicAdapter>();
	NLOG(LOGNLP,Log,TEXT("FLogicAdapterModule constructed"));
}

FLogicAdapterModule::~FLogicAdapterModule()
{
	NLOG(LOGNLP,Log,TEXT("FLogicAdapterModule destructed"));
}

void FLogicAdapterModule::ModuleConfig(const SConfig& InConfig)
{
	Config = InConfig;
}

void FLogicAdapterModule::Handle(const std::string& Input,std::string& Output,const SConfig& InConfig)
{
	ModuleConfig(InConfig);
	if(Config.bMath)
	{
		if(Loap_Math->Process(Input,Output))
		{
			return;
		}
	}
	if(Config.bWeather)
	{
		if(Loap_Weather->Process(Input,Output))
		{
			return;
		}
	}
	if(Config.bTime)
	{
		if(Loap_Time->Process(Input,Output))
		{
			return;
		}
	}
	if(Config.bBase) {}
	// todo BaseLogicAdapter
}
