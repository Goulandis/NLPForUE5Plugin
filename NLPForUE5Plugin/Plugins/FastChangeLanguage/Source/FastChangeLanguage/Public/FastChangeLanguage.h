// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FFastChangeLanguageModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	//用于初始化Py脚本
	void Tick(const float InDeltaTime);
	bool bHasTicked = false;
	FDelegateHandle TickHandle;
};
