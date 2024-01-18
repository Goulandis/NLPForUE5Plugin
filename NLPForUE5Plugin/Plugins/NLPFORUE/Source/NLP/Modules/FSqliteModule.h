#pragma once
#include "CoreMinimal.h"
#include "../SQLite3/sqlite3.h"
#include "NLP/Common/GlobalManager.h"

class NLP_API FSqliteModule
{
public:
	struct SConfig
	{
		SConfig(std::string Dir = "Default",std::string TableName = "statement"):
			CorpusDir(Dir),MainTableName(TableName){}
		std::string CorpusDir;
		std::string MainTableName;
	};
	
	FSqliteModule();
	~FSqliteModule();

	void Handle(const std::string& Input,std::string& Output,const SConfig& InConfig = SConfig());
	void ModuleConfig(const SConfig& InConfig);
	void Train();
private:
	void TrainSelf();
	void GetCorpusFiles(const std::string& CorpusDir,std::vector<std::string>& Files);

	void OpenDatebase(std::string Path = "Default",sqlite3* DB = nullptr);
	void CloseDatebase(sqlite3* DB = nullptr);
	void CreateTable(const std::string& Name = "statement");
	void InsertToTable(const std::string& Text,const std::string& SearchText,const std::string& Conversation,const std::string& InReponseTo);
	void SelectAllSearchTextCol();

	static int SelectAllSearchTextColCallback(void* ThisObj,int Argc,char** Argv,char** AzColName);
private:
	std::string DatabasePath;
	sqlite3*  Database;
	SConfig Config;

	std::vector<std::pair<std::string,std::string>> SearchTextVec;
};

