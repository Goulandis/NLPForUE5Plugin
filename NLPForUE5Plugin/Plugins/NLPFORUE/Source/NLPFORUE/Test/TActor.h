// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NLP/Preprossors/PreprocessorFactory.h"
#include "NLP/Preprossors/FLanguageJudgmentPreprocessor.h"
#include "NLP/Preprossors/FSpecialSymbolPreprocessor.h"
#include "NLP/Preprossors/FStopWordFilteringPreprocessor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TActor.generated.h"

UCLASS()
class NLPFORUE_API ATActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	ELanguageType GetLanguageType();
	UFUNCTION(BlueprintCallable,Category="TActor")
	FString DeteleSpecialSymbol(FString Text);
	UFUNCTION(BlueprintCallable,Category="TActor")
	FString CWS(FString Text);
	UFUNCTION(BlueprintCallable,Category="TActor")
	TArray<FString> StopWordFiltering(FString Text);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
