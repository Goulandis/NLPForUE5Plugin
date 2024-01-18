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
	nlohmann::json SpecialSymbolPreprocessor;
	nlohmann::json WeahterLogicAdapterConfig;
	nlohmann::json MathLogicAdapterConfig;
	nlohmann::json SqliteModuleConfig;

	char* CorpusSplitChr = "	";
private:
	ConfigManager();
	void InitConfigObject();
};
