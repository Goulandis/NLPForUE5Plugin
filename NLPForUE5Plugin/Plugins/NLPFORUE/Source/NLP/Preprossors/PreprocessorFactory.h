// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FLanguageJudgmentPreprocessor.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PreprocessorFactory.generated.h"

/**
 * 
 */
UCLASS()
class NLP_API UPreprocessorFactory : public UObject
{
	GENERATED_BODY()
	
public:
	~UPreprocessorFactory();
	static TSharedPtr<UPreprocessorFactory> CreateInstance();
	// UPreprocessorFactory(const UPreprocessorFactory&) = delete;
	// const UPreprocessorFactory& operator=(const UPreprocessorFactory&) = delete;
	
	template<typename T>
	T& GetPreprocessor()
	{
		return T::CreateInstance();
	}

private:
	UPreprocessorFactory();
	static TSharedPtr<UPreprocessorFactory> Instance;
};


