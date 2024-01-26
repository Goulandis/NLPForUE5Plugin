#include "FSqliteModule.h"
#include <io.h>
#include "FPreprocessorModule.h"

FSqliteModule::FSqliteModule()
{
}

FSqliteModule::~FSqliteModule()
{
}

void FSqliteModule::Handle(const std::string& Input, std::string& Output, const SConfig& InConfig)
{
	OpenDatebase();
	SelectAllSearchTextCol();
	NLOG(LOGNLP,Log,TEXT("%d"),SearchTextVec.size());
	CloseDatebase();
}

void FSqliteModule::ModuleConfig(const SConfig& InConfig)
{
	Config = InConfig;
}

void FSqliteModule::Train()
{
	std::thread TrainThread(std::bind(&FSqliteModule::TrainSelf,this));
	TrainThread.detach();
}

void FSqliteModule::TrainSelf()
{
	NLOG(LOGNLP,Log,TEXT("[%s]Start training"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())));
	CreateTable();
	std::vector<std::string> Files;
	std::string Path;
	if(Config.CorpusDir == "Default")
	{
		Path = GlobalManager::RESOURCE_ABSOLUTE_PATH + std::string(ConfigManager::CreateInstance().SqliteModuleConfig.at("DefaultCorpusPath"));
	}
	std::ifstream Ifs;
	int Err = 0;
	char* ErrMsg;
	FPreprocessorModule* MPrep = new FPreprocessorModule();
	// 读取语料目录下的所有语料文件
	GetCorpusFiles(Path,Files);
	OpenDatebase();
	// 开启SQLite3事务机制
	Err = sqlite3_exec(Database,"begin;",0,0,&ErrMsg);
	// 如果SQLite3事务机制开启失败，则退出训练
	if(Err)
	{
		NLOG(LOGNLP,Error,TEXT("[FSqliteModule::TrainSelf]%s"),*TOFS(std::string(sqlite3_errmsg(Database))));
		CloseDatebase();
		Ifs.close();
		return;
	}
	for(std::string FilePath : Files)
	{
		Ifs.open(FilePath);
		if(!Ifs.is_open())
		{
			continue;
		}
		NLOG(LOGNLP,Log,TEXT("[%s]Reading %s"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())),*TOFS(FilePath));
		std::string Line;
		// 读取单个文件语料
		while (std::getline(Ifs,Line))
		{
			std::vector<std::string> SplitVec = GlobalManager::SplitString(Line,ConfigManager::CreateInstance().CorpusSplitChr);
			if(SplitVec.size() == 2)
			{
				std::string Text = SplitVec[0];
				std::string SearchText = MPrep->Prep_SensitiveWord->SensitiveWordFiltering(Text);
				// 判断输入语料是否包含敏感词，如果包含丢弃语料
				if(Text != SearchText || SearchText.empty()) continue;
				SearchText = MPrep->Prep_SpecialSymbol->DeteleSpecialSymbol(SearchText);
				std::string Flag = ConfigManager::CreateInstance().SqliteModuleConfig.at("DefaultTrainFlag");
				// 数据库插入
				InsertToTable(Text,SearchText,Flag,SplitVec[1]);
			}
		}
		Ifs.close();
		NLOG(LOGNLP,Log,TEXT("[%s]Completed %s"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())),*TOFS(FilePath));
	}
	// 关闭SQLite3事务机制
	Err = sqlite3_exec(Database,"commit;",0,0,&ErrMsg);
	if(Err)
	{
		NLOG(LOGNLP,Error,TEXT("[FSqliteModule::TrainSelf]%s"),*TOFS(std::string(sqlite3_errmsg(Database))));
		CloseDatebase();
		return;
	}
	CloseDatebase();
	delete MPrep;
	NLOG(LOGNLP,Log,TEXT("[%s]End of training"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())));
}

void FSqliteModule::GetCorpusFiles(const std::string& CorpusDir,std::vector<std::string>& Files)
{
	// 存储文件信息的结构体
	_finddata_t FileInfo;
	// 存储路径的字符串
	std::string Tmp;
	// 保存路径的唯一句柄，可以是long,long long和std::intptr_t,在Win10下能使用long，否则会报读取异常
	// _findfirst()查找路径下的第一个文件，成功放回0，失败返回-1
	std::intptr_t hFile = _findfirst(Tmp.assign(CorpusDir).append("/*").c_str(),&FileInfo);
	// hFile=-1表示路径查找失败
	if(hFile == -1) return;
	do
	{
		 // 判断当前文件信息是否是文件夹
		 if(FileInfo.attrib & _A_SUBDIR)
		 {
		 	// 排除当前文件信息是'.'(当前目录)和'..'(当前目录的父目录)的递归，否则将无线循环递归
		 	if(strcmp(FileInfo.name,".") != 0 && strcmp(FileInfo.name,"..") != 0)
		 	{
		 		GetCorpusFiles(Tmp.assign(CorpusDir).append("/").append(FileInfo.name),Files);
		 	}
		 }
		// 如果当前文件信息不是文件夹，则加入列表
		else
		{
			Files.push_back(Tmp.assign(CorpusDir).append("/").append(FileInfo.name));
		}
	}
	// _findnext()查找路径下的下一个文件信息，成功放回0，失败返回-1
	while (_findnext(hFile,&FileInfo) == 0);
	// 关闭查找
	_findclose(hFile);
}

void FSqliteModule::OpenDatebase(std::string Path,sqlite3* DB)
{
	if(Path == "Default")
	{
		Path = GlobalManager::RESOURCE_ABSOLUTE_PATH + std::string(ConfigManager::CreateInstance().SqliteModuleConfig.at("DefaultDatabasePath"));
	}
	if(DB == nullptr)
	{
		int Err = sqlite3_open(Path.c_str(),&Database);
		if(Err)
		{
			NLOG(LOGNLP,Error,TEXT("[FSqliteModule::OpenDatebase]%s"),*TOFS(std::string(sqlite3_errmsg(Database))));
		}
	}
	else
	{
		int Err = sqlite3_open(Path.c_str(),&DB);
		if(Err)
		{
			NLOG(LOGNLP,Error,TEXT("[FSqliteModule::OpenDatebase]%s"),*TOFS(std::string(sqlite3_errmsg(DB))));
		}
	}
}

void FSqliteModule::CloseDatebase(sqlite3* DB)
{
	if(DB == nullptr)
	{
		int Err = sqlite3_close(Database);
		if(Err)
		{
			NLOG(LOGNLP,Error,TEXT("[FSqliteModule::CloseDatebase]%s"),*TOFS(std::string(sqlite3_errmsg(Database))));
		}
	}
	else
	{
		int Err = sqlite3_close(DB);
		if(Err)
		{
			NLOG(LOGNLP,Error,TEXT("[FSqliteModule::CloseDatebase]%s"),*TOFS(std::string(sqlite3_errmsg(DB))));
		}
	}
}

void FSqliteModule::CreateTable(const std::string& Name)
{
	OpenDatebase();
	char* ErrMsg;
	int Err;
	std::string SqlDelte = "DROP TABLE IF EXISTS " + Name;
	Err = sqlite3_exec(Database,SqlDelte.c_str(),nullptr,0,&ErrMsg);
	if(Err != SQLITE_OK)
	{
		NLOG(LOGNLP,Warning,TEXT("[FSqliteModule::CreateTable]%s"),*TOFS(std::string(ErrMsg)));
	}
	std::string Sql = "CREATE TABLE IF NOT EXISTS "+ Name + " ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
			"text NTEXT	NOT NULL,"
			"search_text NTEXT	NOT NULL,"
			"conversation CHAR(20),"
			"created_at	 DATETIME,"
			"in_response_to	NTEXT NOT NULL);";
	Err = sqlite3_exec(Database,Sql.c_str(),nullptr,0,&ErrMsg);
	if(Err != SQLITE_OK)
	{
		NLOG(LOGNLP,Warning,TEXT("[FSqliteModule::CreateTable]%s"),*TOFS(std::string(ErrMsg)));
	}
	CloseDatebase();
}

void FSqliteModule::InsertToTable(const std::string& Text, const std::string& SearchText,const std::string& Conversation, const std::string& InReponseTo)
{
	char* Sql = sqlite3_mprintf("INSERT INTO statement (text,search_text,conversation,created_at,in_response_to) "
					"VALUES ('%q','%q','%q',DATETIME('NOW', 'LOCALTIME'),'%q');",Text.c_str(),SearchText.c_str(),Conversation.c_str(),InReponseTo.c_str());
	char* ErrMsg;
	int Err = sqlite3_exec(Database,Sql,nullptr,0,&ErrMsg);
	if(Err)
	{
		NLOG(LOGNLP,Error,TEXT("[FSqliteModule::InsertToTable]%s"),*TOFS(std::string(sqlite3_errmsg(Database))));
	}
}

void FSqliteModule::SelectAllSearchTextCol()
{
	char* Sql = sqlite3_mprintf("SELECT id,search_text FROM %q",Config.MainTableName.c_str());
	char* ErrMsg;
	int Err = sqlite3_exec(Database,Sql,FSqliteModule::SelectAllSearchTextColCallback,this,&ErrMsg);
	if(Err)
	{
		NLOG(LOGNLP,Error,TEXT("[FSqliteModule::SelectAllSearchTextCol]%s"),*TOFS(std::string(sqlite3_errmsg(Database))));
	}
}

int FSqliteModule::SelectAllSearchTextColCallback(void* ThisObj, int Argc, char** Argv, char** AzColName)
{
	if(Argc == 2)
	{
		((FSqliteModule*)ThisObj)->SearchTextVec.push_back(std::pair<std::string,std::string>(Argv[0],Argv[1]));
		return 0;
	}
	return 1;
}
