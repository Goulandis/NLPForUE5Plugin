// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NLP/Preprossors/FLanguageJudgmentPreprocessor.h"
#include "NLP/Preprossors/FSensitiveWordPreprocessor.h"
#include "NLP/Preprossors/FSpecialSymbolPreprocessor.h"
#include "NLP/Preprossors/FStopWordFilteringPreprocessor.h"
#include "CoreMinimal.h"

class NLP_API FPreprocessorModule
{
public:
	struct SConfig
	{
		SConfig(bool laj = true,bool sew = true,bool sps = true,bool stw = true):
			bLanguageJudgment(laj),bSensitiveWord(sew),bSpecialSymbol(sps),bStopWord(stw){}
		bool bLanguageJudgment = true;
		bool bSensitiveWord = true;
		bool bSpecialSymbol = true;
		bool bStopWord = true;
	};
	
	FPreprocessorModule();
	~FPreprocessorModule();
	
	void Handle(const std::string& Input,std::string& Output,const SConfig& InConfig = SConfig());
	void ModuleConfig(const SConfig& InConfig);

	FLanguageJudgmentPreprocessor* Prep_LanguageJudgment;
	FSensitiveWordPreprocessor* Prep_SensitiveWord;
	FSpecialSymbolPreprocessor* Prep_SpecialSymbol;
	FStopWordFilteringPreprocessor* Prep_StopWord;
	
private:
	template<typename T>
	static T& GetPreprocessor()
	{
		return T::Get();
	}
	
	SConfig Config;
};


