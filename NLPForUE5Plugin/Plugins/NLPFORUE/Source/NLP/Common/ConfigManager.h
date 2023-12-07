#pragma once
#include "CoreMinimal.h"
#include "nlohmann/json.hpp"

class NLP_API ConfigManager
{
public:
	static ConfigManager& CreateInstance();
	~ConfigManager();

	nlohmann::json Config;

	nlohmann::json SensitiveWordPreprocessorConfig;
	nlohmann::json WeahterLogicAdapterConfig;
	nlohmann::json MathLogicAdapterConfig;
private:
	ConfigManager();
	void InitConfigObject();
};
