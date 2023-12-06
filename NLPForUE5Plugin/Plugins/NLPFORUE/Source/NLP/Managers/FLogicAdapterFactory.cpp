#include "FLogicAdapterFactory.h"
//#include "NLP/Common/GlobalManager.h"

FLogicAdapterFactory* FLogicAdapterFactory::Instance = nullptr;

// void FLogicAdapterFactory::DestroyInstance()
// {
// 	if(Instance->BaseLogicAdapter)
// 	{
// 		//delete Instance->BaseLogicAdapter;
// 		Instance->BaseLogicAdapter = nullptr;
// 	}
// 	if(Instance->MathLogicAdapter)
// 	{
// 		//delete Instance->MathLogicAdapter;
// 		Instance->MathLogicAdapter = nullptr;
// 	}
// 	if(Instance->TimeLogicAdapter)
// 	{
// 		//delete Instance->TimeLogicAdapter;
// 		Instance->TimeLogicAdapter = nullptr;
// 	}
// 	if(Instance->WeatherLogicAdapter)
// 	{
// 		//delete Instance->WeatherLogicAdapter;
// 		Instance->WeatherLogicAdapter = nullptr;
// 	}
// 	// if(Instance)
// 	// {
// 	// 	delete Instance;
// 	// 	Instance = nullptr;
// 	// }
// }

FLogicAdapterFactory::FLogicAdapterFactory()
{
	NLOG(LOGNLP,Log,TEXT("FLogicAdapterFactory constructed"));
}

FLogicAdapterFactory::~FLogicAdapterFactory()
{
	NLOG(LOGNLP,Log,TEXT("FLogicAdapterFactory destructed"));
}

FLogicAdapterFactory* FLogicAdapterFactory::CreateInstance()
{
	if(!Instance)
	{
		Instance = new FLogicAdapterFactory();
		Instance->BaseLogicAdapter = &GetLogicAdapterSelf<FBaseLogicAdapter>();
		Instance->MathLogicAdapter = &GetLogicAdapterSelf<FMathLogicAdapter>();
		Instance->TimeLogicAdapter = &GetLogicAdapterSelf<FTimeLogicAdapter>();
		Instance->WeatherLogicAdapter = &GetLogicAdapterSelf<FWeatherLogicAdapter>();
	}
	return Instance;
}
