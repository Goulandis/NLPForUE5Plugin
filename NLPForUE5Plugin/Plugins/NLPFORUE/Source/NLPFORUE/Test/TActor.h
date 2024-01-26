// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../NLP/Net/FSoc.h"
#include "NLP/Modules/FPreprocessorModule.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NLP/Modules/FLogicAdapterModule.h"
#include "NLP/Modules/FTrainModule.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> 
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

	// --------------jieba API-------------------
	UFUNCTION(BlueprintCallable,Category="TActor")
	FString CWS(FString Text);
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

	// --------------Soc API---------------------
	UFUNCTION(BlueprintCallable,Category="TActor")
	bool SocConnect(FString InIP = "127.0.0.1",int InPort = 7214);
	UFUNCTION(BlueprintCallable,Category="TActor")
	bool SocReconnect();
	UFUNCTION(BlueprintCallable,Category="TActor")
	bool SocSend(FString Msg);
	UFUNCTION(BlueprintCallable,Category="TActor")
	bool SocCmd(FString InCmd,FString InType,FString InData);
	UFUNCTION(BlueprintCallable,Category="TActor")
	void SocClose();
	UFUNCTION(BlueprintCallable,Category="TActor")
	void Word2VecServerClose();
	UFUNCTION(BlueprintCallable,Category="TActor")
	void Word2VecServerSetup();
	UFUNCTION(BlueprintCallable,Category="TActor")
	void Word2VecServerWindowFind();
	UFUNCTION(BlueprintCallable,Category="TActor")
	void Train();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TSharedPtr<FPreprocessorModule> MPrep;
	TSharedPtr<FLogicAdapterModule> MLoap;
	TSharedPtr<FTrainModule> MTrain;
	HWND Word2VecServerWindwosHandle;
};
