#pragma once
#include <type_traits>
#include "NLP/Managers/FPreprocessorFactory.h"
#include "NLP/Preprossors/FLanguageJudgmentPreprocessor.h"
#include "NLP/Preprossors/FSensitiveWordPreprocessor.h"
#include "NLP/Preprossors/FSpecialSymbolPreprocessor.h"
#include "NLP/Preprossors/FStopWordFilteringPreprocessor.h"
#include "CoreMinimal.h"

class NLP_API FPreprocessorFactory
{
public:
	~FPreprocessorFactory();

	static FPreprocessorFactory* CreateInstance();

	template<typename T>
	T* GetPreprocessor()
	{
		if(is_same<T,FLanguageJudgmentPreprocessor>::value) return (T*)(LanguageJudgmentPreprocessor);
		if(is_same<T,FSensitiveWordPreprocessor>::value) return (T*)(SensitiveWordPreprocessor);
		if(is_same<T,FSpecialSymbolPreprocessor>::value) return (T*)(SpecialSymbolPreprocessor);
		if(is_same<T,FStopWordFilteringPreprocessor>::value) return (T*)(StopWordFilteringPreprocessor);
		return nullptr;
	}
private:
	FPreprocessorFactory();

	template<typename T>
	static T& GetPreprocessorSelf()
	{
		return T::CreateInstance();
	}
	
	static FPreprocessorFactory* Instance;

	FLanguageJudgmentPreprocessor* LanguageJudgmentPreprocessor;
	FSensitiveWordPreprocessor* SensitiveWordPreprocessor;
	FSpecialSymbolPreprocessor* SpecialSymbolPreprocessor;
	FStopWordFilteringPreprocessor* StopWordFilteringPreprocessor;
};
