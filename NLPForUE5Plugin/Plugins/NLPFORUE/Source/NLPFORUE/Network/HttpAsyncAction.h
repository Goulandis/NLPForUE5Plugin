#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "HttpAsyncAction.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHttpResponsDeleget,FString,Data);

UCLASS()
class NLPFORUE_API UHttpAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString URL;
	UPROPERTY()
	FString Data;

	UPROPERTY(BlueprintAssignable)
	FHttpResponsDeleget OnReqSuccess;
	UPROPERTY(BlueprintAssignable)
	FHttpResponsDeleget OnReqFail;

	UFUNCTION(BlueprintCallable,meta=(DisplayName="发起Http请求",Wordkeys="http",BlueprintInternalUseOnly="true"),Category="UHttpAsyncAction")
	static UHttpAsyncAction* HttpRequest(FString InURL,FString InData);
	virtual void Activate() override;
};
