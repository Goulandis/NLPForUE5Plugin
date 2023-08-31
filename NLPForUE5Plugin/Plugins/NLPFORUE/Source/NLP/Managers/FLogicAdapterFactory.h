#pragma once
#include <type_traits>
#include "NLP/LogicAdapters/FBaseLogicAdapter.h"
#include "NLP/LogicAdapters/FMathLogicAdapter.h"
#include "NLP/LogicAdapters/FTimeLogicAdapter.h"
#include "NLP/LogicAdapters/FWeatherLogicAdapter.h"
#include "CoreMinimal.h"

class NLP_API FLogicAdapterFactory
{
public:
	~FLogicAdapterFactory();
	static FLogicAdapterFactory* CreateInstance();

	template<typename T>
	T* GetLogicAdapter()
	{
		if(std::is_same<T,FBaseLogicAdapter>::value) return (T*)(BaseLogicAdapter);
		if(std::is_same<T,FMathLogicAdapter>::value) return (T*)(MathLogicAdapter);
		if(std::is_same<T,FTimeLogicAdapter>::value) return (T*)(TimeLogicAdapter);
		if(std::is_same<T,FWeatherLogicAdapter>::value) return (T*)(WeatherLogicAdapter);
		return nullptr;
	}

	//static void DestroyInstance();
private:
	FLogicAdapterFactory();

	template<typename T>
	static T& GetLogicAdapterSelf()
	{
		return T::CreateInstance();
	}
	
	static FLogicAdapterFactory* Instance;

	FBaseLogicAdapter* BaseLogicAdapter;
	FMathLogicAdapter* MathLogicAdapter;
	FTimeLogicAdapter* TimeLogicAdapter;
	FWeatherLogicAdapter* WeatherLogicAdapter;
};
