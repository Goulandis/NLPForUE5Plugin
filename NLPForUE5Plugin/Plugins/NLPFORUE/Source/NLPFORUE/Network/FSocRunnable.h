#pragma once

#include "Runtime/Networking/Public/Networking.h"
#include "../Common/FDefine.h"
#include "HAL/Runnable.h"
#include "CoreMinimal.h"

DECLARE_DELEGATE_OneParam(FOnRecv,FString);

class NLPFORUE_API FSocRunnable : public FRunnable
{
public:
	FSocRunnable(FSocket* InSocClient):SocClient(InSocClient){};
	~FSocRunnable();
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	bool CreateSocRunnable(const FString& Name);
public:
	FOnRecv OnRecv;
private:
	TSharedPtr<FRunnableThread> RunnableThread;
	FString ThreadName;
	FSocket* SocClient;
	bool Stopping;
	FThreadSafeCounter StopTaskCounter;
};
