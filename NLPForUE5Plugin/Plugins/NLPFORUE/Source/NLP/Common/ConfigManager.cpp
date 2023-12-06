#include "ConfigManager.h"
#include <fstream>
#include "GlobalManager.h"

ConfigManager& ConfigManager::CreateInstance()
{
	static ConfigManager Instance;
	return Instance;
}

ConfigManager::ConfigManager()
{
	
	std::ifstream Ifs(GlobalManager::RESOURCE_ABSOLUTE_PATH + GlobalManager::LOGICADAPTER_CONFIG_PATH);
	if(Ifs.is_open())
	{
		Ifs >> LogicAdapterConfig;
		std::string JsonStr = LogicAdapterConfig.dump();
		UE_LOG(LOGNLP,Log,TEXT("Config:%s"),*TOFSTR(JsonStr));
	}
	else
	{
		UE_LOG(LOGNLP,Log,TEXT("无法打开文件:../../../Resources/Config/LogicAdapter.json"));
	}
}

ConfigManager::~ConfigManager()
{
}
