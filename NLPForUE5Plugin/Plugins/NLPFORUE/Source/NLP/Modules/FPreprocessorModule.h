// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NLP/Managers/FPreprocessorFactory.h"
#include "CoreMinimal.h"

class NLP_API FPreprocessorModule
{
public:
	FPreprocessorModule();
	~FPreprocessorModule();
	
	TArray<FString> Handle(FString& Text) const;
};


