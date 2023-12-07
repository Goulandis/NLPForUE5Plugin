#include "ConfigManager.h"
#include <fstream>
#include "GlobalManager.h"
#include "NLP/LogicAdapters/FWeatherLogicAdapter.h"

ConfigManager& ConfigManager::CreateInstance()
{
	static ConfigManager Instance;
	return Instance;
}

ConfigManager::ConfigManager()
{
	const std::string Path = GlobalManager::RESOURCE_ABSOLUTE_PATH + GlobalManager::CONFIG_PATH;
	std::ifstream Ifs(Path);
	if(Ifs.is_open())
	{
		Ifs >> Config;
		InitConfigObject();
	}
	else
	{
		NLOG(LOGNLP,Error,TEXT("Failed to open the file : %s"),*TOFSTR(Path));
	}
	Ifs.close();
}

void ConfigManager::InitConfigObject()
{
	if(!Config.is_object())
	{
		NLOG(LOGNLP,Error,TEXT("Object Config is not a json : %s"),*TOFSTR(Config.dump()));
		return;
	}
	WeahterLogicAdapterConfig = Config.at("WeatherLogicAdapter");
	MathLogicAdapterConfig = Config.at("MathLogicAdapter");
	SensitiveWordPreprocessorConfig = Config.at("SensitiveWordPreprocessor");
}

ConfigManager::~ConfigManager()
{
}
