#include "FTrainModule.h"
#include <io.h>
#include "FPreprocessorModule.h"
#include "NLP/Managers/FPythonServerManager.h"
#include "Format.h"

FTrainModule::FTrainModule()
{
	Config = SConfig();
}

FTrainModule::~FTrainModule()
{
}

void FTrainModule::Handle(const std::string& Input, std::string& Output, const SConfig& InConfig)
{
}

void FTrainModule::ModuleConfig(const SConfig& InConfig)
{
	Config = InConfig;
}

void FTrainModule::Train()
{
	std::thread TrainThread(std::bind(&FTrainModule::TrainSelf,this));
	TrainThread.detach();
}

void FTrainModule::TrainSelf()
{
	NLOG(LOGNLP,Log,TEXT("[%s]Start training"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())));
	GlobalManager::CreateTable();
	std::vector<std::string> Files;
	std::ifstream Ifs;
	int Err = 0;
	char* ErrMsg;
	FPreprocessorModule* MPrep = new FPreprocessorModule();
	// 读取语料目录下的所有语料文件
	GetCorpusFiles(Config.SqliteCorpusDir,Files);
	// 开启SQLite3事务机制
	Err = sqlite3_exec(GlobalManager::GetDatabase(),"begin;",0,0,&ErrMsg);
	// 如果SQLite3事务机制开启失败，则退出训练
	if(Err)
	{
		NLOG(LOGNLP,Error,TEXT("[FSqliteModule::TrainSelf]%s"),*TOFS(std::string(sqlite3_errmsg(GlobalManager::GetDatabase()))));
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
		std::string ReadLine;
		// 读取单个文件语料
		while (std::getline(Ifs,ReadLine))
		{
			std::vector<std::string> SplitVec = GlobalManager::SplitString(ReadLine,ConfigManager::Get().CorpusSplitChr);
			if(SplitVec.size() == 2)
			{
				std::string Text = SplitVec[0];
				std::string SearchText = MPrep->Prep_SensitiveWord->SensitiveWordFiltering(Text);
				// 判断输入语料是否包含敏感词，如果包含丢弃语料
				if(Text != SearchText || SearchText.empty()) continue;
				SearchText = MPrep->Prep_SpecialSymbol->DeteleSpecialSymbol(SearchText);
				if(SearchText.empty()) continue;
				std::string Flag = ConfigManager::Get().TrainModuleConfig.at("DefaultTrainFlag");
				// 数据库插入
				InsertToTable(Text,SearchText,Flag,SplitVec[1]);
			}
		}
		Ifs.close();
		NLOG(LOGNLP,Log,TEXT("[%s]Completed %s"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())),*TOFS(FilePath));
	}
	// 关闭SQLite3事务机制
	Err = sqlite3_exec(GlobalManager::GetDatabase(),"commit;",0,0,&ErrMsg);
	if(Err)
	{
		NLOG(LOGNLP,Error,TEXT("[FSqliteModule::TrainSelf]%s"),*TOFS(std::string(sqlite3_errmsg(GlobalManager::GetDatabase()))));
		return;
	}
	delete MPrep;
	NLOG(LOGNLP,Log,TEXT("[%s]End of training"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())));
	
	// 生成Word2Vec词向量模型训练语料
	GenerateWord2VecCorpus();
	FPythonServerManager::Get().SocCmd("Word2Vec","Train","{}");
}

void FTrainModule::GenerateWord2VecCorpus()
{
	NLOG(LOGNLP,Log,TEXT("[%s]Start Generating Word2Vec Curpos"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())));
	std::vector<std::string> Files;
	std::ifstream Ifs;
	std::ofstream Ofs;
	FPreprocessorModule* MPrep = new FPreprocessorModule();
	// 读取语料目录下的所有语料文件
	GetCorpusFiles(Config.SqliteCorpusDir,Files);
	for(std::string FilePath : Files)
	{
		Ifs.open(FilePath);
		if(!Ifs.is_open())
		{
			continue;
		}
		NLOG(LOGNLP,Log,TEXT("[%s]Reading %s"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())),*TOFS(FilePath));

		// 生成Word2Vec模型训练语料的存储文件
		std::string WritePath = Config.Word2VecCorpusDir + FilePath.substr(Config.SqliteCorpusDir.length(),FilePath.length());
		NLOG(LOGNLP,Log,TEXT("[%s]Generating file : %s"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())),*TOFS(WritePath));
		// 文件不存在则创建，文件存在且有内容则清空
		Ofs.open(WritePath,ios::trunc);
		
		std::string ReadLine;
		// 读取单个文件语料
		while (std::getline(Ifs,ReadLine))
		{
			std::vector<std::string> SplitVec = GlobalManager::SplitString(ReadLine,ConfigManager::Get().CorpusSplitChr);
			for(std::string Text : SplitVec)
			{
				if(MPrep->Prep_SensitiveWord->HasSensitiveWord(Text)) continue;
				Text = MPrep->Prep_SpecialSymbol->DeteleSpecialSymbol(Text);
				std::vector<std::string> WordVec;
				GlobalManager::jieba.Cut(Text, WordVec);
				std::string WriteLine;
				for (std::string Word : WordVec)
				{
					WriteLine += Word + " ";
				}
				// 使用\n作为包与包之间的间隔符，避免TCP的粘包现象
				WriteLine += "\n";
				Ofs << WriteLine;
			}
		}
		Ifs.close();
		Ofs.close();
		NLOG(LOGNLP,Log,TEXT("[%s]Generated file Completed : %s"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())),*TOFS(FilePath));
	}
	delete MPrep;
	NLOG(LOGNLP,Log,TEXT("[%s]Word2Vec corpus generated"),*TOFS(GlobalManager::TmToString(GlobalManager::GetNowLocalTime())));
}

void FTrainModule::GetCorpusFiles(const std::string& CorpusDir,std::vector<std::string>& Files)
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



void FTrainModule::InsertToTable(const std::string& Text, const std::string& SearchText,const std::string& Conversation, const std::string& InReponseTo)
{
	char* Sql = sqlite3_mprintf("INSERT INTO statement (text,search_text,conversation,created_at,in_response_to) "
					"VALUES ('%q','%q','%q',DATETIME('NOW', 'LOCALTIME'),'%q');",Text.c_str(),SearchText.c_str(),Conversation.c_str(),InReponseTo.c_str());
	char* ErrMsg;
	int Err = sqlite3_exec(GlobalManager::GetDatabase(),Sql,nullptr,0,&ErrMsg);
	if(Err)
	{
		NLOG(LOGNLP,Error,TEXT("[FSqliteModule::InsertToTable]%s"),*TOFS(std::string(sqlite3_errmsg(GlobalManager::GetDatabase()))));
	}
}

void FTrainModule::SelectAllSearchTextCol()
{
	char* Sql = sqlite3_mprintf("SELECT id,search_text FROM %q",Config.MainTableName.c_str());
	char* ErrMsg;
	int Err = sqlite3_exec(Database,Sql,FTrainModule::SelectAllSearchTextColCallback,this,&ErrMsg);
	if(Err)
	{
		NLOG(LOGNLP,Error,TEXT("[FSqliteModule::SelectAllSearchTextCol]%s"),*TOFS(std::string(sqlite3_errmsg(Database))));
	}
}

int FTrainModule::SelectAllSearchTextColCallback(void* ThisObj, int Argc, char** Argv, char** AzColName)
{
	if(Argc == 2)
	{
		((FTrainModule*)ThisObj)->SearchTextVec.push_back(std::pair<std::string,std::string>(Argv[0],Argv[1]));
		return 0;
	}
	return 1;
}
