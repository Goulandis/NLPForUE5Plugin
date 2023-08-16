#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


class FNLP : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
