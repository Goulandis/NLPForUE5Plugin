// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NLP/Managers/FPreprocessorFactory.h"
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
	
	void Handle(const std::string& Input,std::string& Output,const SConfig& InConfig);
	void ModuleConfig(const SConfig& InConfig);

	FLanguageJudgmentPreprocessor* Prep_LanguageJudgment;
	FSensitiveWordPreprocessor* Prep_SensitiveWord;
	FSpecialSymbolPreprocessor* Prep_SpecialSymbol;
	FStopWordFilteringPreprocessor* Prep_StopWord;
	
private:
	SConfig Config;
};


