#pragma once
#include "CoreMinimal.h"
#include "../SQLite3/sqlite3.h"
#include "NLP/Common/GlobalManager.h"

class NLP_API FTrainModule
{
public:
	struct SConfig
	{
		SConfig(std::string InSqliteCorpusDir = "Default",std::string InMainTableName = "statement",std::string InWord2VecCorpusDir = "Default")
		{
			SqliteCorpusDir = InSqliteCorpusDir;
			MainTableName = InMainTableName;
			Word2VecCorpusDir = InWord2VecCorpusDir;
			if(InSqliteCorpusDir == "Default")
			{
				SqliteCorpusDir = GlobalManager::RESOURCE_ABSOLUTE_PATH + std::string(ConfigManager::Get().TrainModuleConfig.at("DefaultSqliteCorpusPath"));
			}
			if(InWord2VecCorpusDir == "Default")
			{
				Word2VecCorpusDir = GlobalManager::RESOURCE_ABSOLUTE_PATH + std::string(ConfigManager::Get().TrainModuleConfig.at("DefaultWord2VecCorpusPath"));
			}
		}
		std::string SqliteCorpusDir;
		std::string MainTableName;
		std::string Word2VecCorpusDir;;
	};
	
	FTrainModule();
	~FTrainModule();

	void Handle(const std::string& Input,std::string& Output,const SConfig& InConfig = SConfig());
	void ModuleConfig(const SConfig& InConfig);
	void Train();
private:
	void TrainSelf();
	void GenerateWord2VecCorpus();
	void GetCorpusFiles(const std::string& CorpusDir,std::vector<std::string>& Files);
	
	void InsertToTable(const std::string& Text,const std::string& SearchText,const std::string& Conversation,const std::string& InReponseTo);
	void SelectAllSearchTextCol();

	static int SelectAllSearchTextColCallback(void* ThisObj,int Argc,char** Argv,char** AzColName);

	std::string DatabasePath;
	sqlite3*  Database;
	SConfig Config;

	std::vector<std::pair<std::string,std::string>> SearchTextVec;
};

