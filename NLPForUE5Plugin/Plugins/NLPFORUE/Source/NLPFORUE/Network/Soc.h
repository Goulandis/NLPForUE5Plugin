#pragma once

#include "FSocRunnable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Soc.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecvMsg,FString,Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConnected);

UCLASS()
class NLPFORUE_API ASoc : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoc();
	~ASoc();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;
public:	
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable,Category="NLPFORUE|Network")
	bool SendMsg(const FString& Msg);
	UFUNCTION(BlueprintCallable,Category="NLPFORUE|Network")
	bool SendCmd(const FString& Cmd,const FString& Type,const FString& Data);
	void OnReceived(FString Data);
	UFUNCTION(BlueprintCallable,Category="NLPFORUE|Network")
	void ConnetLTP(const FString& InIP = "127.0.0.1",const int32 InPort = 7214,const FString& SocDescription = "tlp client");
	UFUNCTION(BlueprintCallable,Category="NLPFORUE|Network")
	void SocClose();
	UFUNCTION(BlueprintCallable,Category="NLPFORUE|Network")
	FString CmdFormat(const FString& Cmd,const FString& Type,const FString& Data);
	//中英文混合utf-8编码字符串字节数量计算
	int32 CalcUtf0NumFromString(const FString& Str);
public:
	UPROPERTY(BlueprintAssignable)
	FOnRecvMsg OnRecvMsg;
	UPROPERTY(BlueprintAssignable)
	FOnConnected OnConnected;
private:
	FSocket* SocClient;
	TSharedPtr<FSocRunnable> SocRunnable;
};
