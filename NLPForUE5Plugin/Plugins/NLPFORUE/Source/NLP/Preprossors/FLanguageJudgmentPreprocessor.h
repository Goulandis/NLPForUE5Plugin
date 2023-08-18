// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FPreprossorsType.h"
#include "FPreprocessor.h"
#include "CoreMinimal.h"

using namespace std;

class NLP_API FLanguageJudgmentPreprocessor : public FPreprocessor
{
public:
	static FLanguageJudgmentPreprocessor& CreateInstance();

	// 判断输入文本是英文还是中文，目前只支持中英文识别
	ELanguageType GetLanguageType(std::string& Text);
	ELanguageType GetLanguageType(FString Text);

	~FLanguageJudgmentPreprocessor();
private:
	FLanguageJudgmentPreprocessor();
	
	//FLanguageJudgmentPreprocessor(const FLanguageJudgmentPreprocessor&) = delete;
	//const FLanguageJudgmentPreprocessor& operator=(const FLanguageJudgmentPreprocessor&) = delete;
};
