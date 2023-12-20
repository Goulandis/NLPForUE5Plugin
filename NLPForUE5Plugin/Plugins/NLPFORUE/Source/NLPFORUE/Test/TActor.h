// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NLP/Modules/FPreprocessorModule.h"
#include "NLP/Managers/FLogicAdapterFactory.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NLP/Modules/FLogicAdapterModule.h"
#include "TActor.generated.h"

UCLASS()
class NLPFORUE_API ATActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATActor();
	~ATActor();
	virtual void BeginDestroy() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	GlobalManager::ELanguageType GetLanguageType();
	UFUNCTION(BlueprintCallable,Category="TActor")
	FString CWS(FString Text);
	UFUNCTION(BlueprintCallable,Category="TActor")
	FString SensitiveWordFiltering(FString Text);
	UFUNCTION(BlueprintCallable,Category="TActor")
	FString TestJiebaTag(FString Text);
	UFUNCTION(BlueprintCallable,Category="TActor")
	FString TestJiebaExtract(FString Text);
	UFUNCTION(BlueprintCallable,Category="TActor")
	void PreprocessorTest(FString Text);
	UFUNCTION(BlueprintCallable,Category="TActor")
	FString PrepAndLoapConformity(FString Text);

	UFUNCTION(BlueprintCallable,Category="TActor")
	FString ComTest(FString Text);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TSharedPtr<FPreprocessorModule> MPrep;
	TSharedPtr<FLogicAdapterModule> MLoap;
};
