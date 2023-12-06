#pragma once
#include "CoreMinimal.h"
#include "nlohmann/json.hpp"

class NLP_API ConfigManager
{
public:
	static ConfigManager& CreateInstance();
	~ConfigManager();
private:
	ConfigManager();

	nlohmann::json LogicAdapterConfig;
	nlohmann::json PreprocessorConfig;
};
