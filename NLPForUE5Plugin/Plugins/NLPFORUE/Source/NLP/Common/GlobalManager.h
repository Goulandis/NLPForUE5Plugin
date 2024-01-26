#ifndef GLOBALMANAGER
#define GLOBALMANAGER

#include <regex>
#include <time.h>
#include <string>
#include <vector>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "cpp-httplib/httplib.h"
#include "cppjieba/Jieba.hpp"
#include "../SQLite3/sqlite3.h"
#include "UEManager.h"
#include "ConfigManager.h"
#include "Math/UnitConversion.h"

// 词性宏
#define CX_M "m" // 数词
#define CX_T "t" // 时间词
#define CX_I "i" // 成语
#define CX_N "n" // 名词
#define CX_V "v" // 动词

namespace GlobalManager
{
	// 算式标识
	enum class OpeTag
	{
		Add,// 加法
		Sub,// 减法
		Mul,// 乘法
		Div,// 除法
		Rem,// 求余
		Squ,// 平方
		Cub,// 立方
		Squr,// 平方根
		Cubr,// 立方根
		Pow,// 求幂
		Rot,// 取根
	};

	enum class ELanguageType
	{
		None,
		zh_CN,//简体中文
		en_US,//英文(美国)
		zh_HK//繁体中文(香港)
	};

	// 资源路径
	const char* const RESOURCE_PATH = "NLPFORUE/Resources/";
	const char* const SOURCE_PATH = "NLPFORUE/Source/";
	// 全局配置文件相对路径
	const char* const CONFIG_PATH = "Config/Config.json";
	// 结巴分词字典文件相对路径
	const char* const DICT_PATH = "cppjieba/dict/jieba.dict.utf8";
	// 结巴分词hmm模型配置文件相对路径
	const char* const HMM_PATH = "cppjieba/dict/hmm_model.utf8";
	// 结巴分词用户词典文件相对路径
	const char* const USER_DICT_PATH = "cppjieba/dict/user.dict.utf8";
	// 结巴分词词语权重文件相对路径
	const char* const IDF_PATH = "cppjieba/dict/idf.utf8";
	// 结巴分词停用词文件相对路径
	const char* const STOP_WORD_PATH = "cppjieba/dict/stop_words.utf8";
	
	// 结巴分词库全局实例
	inline cppjieba::Jieba jieba(
		TOUTF8(*(PROJECTPLUGINDIR + SOURCE_PATH + DICT_PATH)),
		TOUTF8(*(PROJECTPLUGINDIR + SOURCE_PATH + HMM_PATH)),
		TOUTF8(*(PROJECTPLUGINDIR + SOURCE_PATH + USER_DICT_PATH)),
		TOUTF8(*(PROJECTPLUGINDIR + SOURCE_PATH + IDF_PATH)),
		TOUTF8(*(PROJECTPLUGINDIR + SOURCE_PATH + STOP_WORD_PATH))
	);
	// 全局Sqlite3数据库对象
	inline sqlite3*  Database = nullptr;
	inline std::string DatabaseTableName = "statement";
	// 判断字符串是否匹配数学算式
	inline bool RegexMathFormulas(const std::string& Text)
	{
		std::regex rp(R"((\d+\s*[\+\-\*/\^%]\s*)+\d+)");
		return std::regex_search(Text,rp);
	}
	// 判断一个词是否是数字
	inline bool IsNumber(const std::string& Word)
	{
		
		//return std::regex_match(Word,rp);
		std::vector<std::pair<std::string,std::string>> Tagers;
		jieba.Tag(Word,Tagers);
		bool Rel = false;
		std::regex ExcPattren(R"(一下|一次|多少|余|二次|三次|几|年|月|日|号)");
		std::regex NumPattren(R"(\d+)");
		for(std::pair<std::string,std::string> Pai : Tagers)
		{
			if((Pai.second == "m" && !std::regex_search(Pai.first,ExcPattren)) || std::regex_search(Pai.first,NumPattren))
			{
				Rel = true;
			}
			else
			{
				Rel = false;
				break;
			}
		}
		return Rel;
	}
	// 判断一句话是否包含中文
	inline bool IsChinese(const std::string& Text)
	{
		const char* TChar = Text.data();
		char Chr;
		while(true)
		{
			Chr = *TChar++;
			// 如果字符串直到结尾都没有出现中文字符，则认为字符串为其他语言
			if(Chr == 0)
			{
				break;
			}
			// 如果字符串中出现了中文，则认为字符串为中文
			if((Chr & 0x80) && (*TChar & 0x80))
			{
				return true;
			}
		}
		return false;
	}
	// 分割字符串
	inline std::vector<std::string> SplitString(std::string Str,const char* Separator)
	{
		char* ptr = NULL;
		char* p = strtok_s(Str.data(),Separator,&ptr);
		std::vector<std::string> LineVec;
		while (p)
		{
			LineVec.push_back(p);
			p = strtok_s(NULL,Separator,&ptr);
		}
		return LineVec;
	}
	inline std::tm TimeOperator(const std::tm& Time1,const std::tm& Time2,const OpeTag Ope = OpeTag::Add)
	{
		std::tm RelTime = {0};
		std::map<int,int> MonMap = {
			{1,31},{3,31},{4,30},{5,31},{6,30},{7,31},{8,31},{9,30},{10,31},{11,30},{12,31}
		};
		// 年份能被100整除，则年份为世纪年
		if(Time1.tm_year%100==0)
		{
			// 世纪年能被400整除，则为闰年，否则不是闰年
			if(Time1.tm_year%400==0)
			{
				MonMap.insert(std::pair<int,int>(2,29));
			}
			else
			{
				MonMap.insert(std::pair<int,int>(2,28));
			}
		}
		else
		{
			// 非世纪年能被4整除，则为闰年，否则不是闰年
			if(Time1.tm_year%4==0)
			{
				MonMap.insert(std::pair<int,int>(2,29));
			}
			else
			{
				MonMap.insert(std::pair<int,int>(2,28));
			}
		}

		int Sec=0,Min=0,Hour=0,Day=0,Mon=0,Year=0;
		bool bSec = false,bMin = false,bHour = false,bDay = false,bMon = false;
		if(Ope == OpeTag::Add)
		{
			Sec = Time1.tm_sec + Time2.tm_sec;
			if(Sec >= 60)
			{
				Sec -= 60;
				bSec = true;
			}
			Min = Time1.tm_min + Time2.tm_min;
			if(bSec){Min += 1;bSec=false;}
			if(Min >= 60)
			{
				Min -= 60;
				bMin = true;
			}
			Hour = Time1.tm_hour + Time2.tm_hour;
			if(bMin){Hour+=1;bMin=false;}
			if(Hour >= 24)
			{
				Hour -= 24;
				bHour = true;
			}
			Day = Time1.tm_mday + Time2.tm_mday;
			if(bHour){Day+=1;bHour=false;}
			if(Day>=MonMap[Time1.tm_mon])
			{
				Day -= MonMap[Time1.tm_mon];
				bDay = true;
			}
			Mon = Time1.tm_mon + Time2.tm_mon;
			if(bDay){Mon+=1;bDay=false;}
			if(Mon > 12)
			{
				Mon -= 12;
				bMon = true;
			}
			Year = Time1.tm_year + Time2.tm_year;
			if(bMon){Year+=1;bMon=false;}
		}
		if(Ope == OpeTag::Sub)
		{
			Sec = Time1.tm_sec - Time2.tm_sec;
			if(Sec < 0)
			{
				bSec = true;
			}
			Min = Time1.tm_min - Time2.tm_min;
			if(bSec)
			{
				Min--;
				Sec = 60 + Sec;//Sec<0
				bSec = false;
			}
			bMin = Min<0?true:false;
			Hour = Time1.tm_hour - Time2.tm_hour;
			if(bMin)
			{
				Hour--;
				Min = 60 + Min;//Min<0
				bMin = false;
			}
			bHour = Hour<0?true:false;
			Day = Time1.tm_mday - Time2.tm_mday;
			if(bHour)
			{
				Day--;
				Hour = 24 + Hour;//Hour<0
				bHour = false;
			}
			bDay = Day<1?true:false;
			Mon = Time1.tm_mon - Time2.tm_mon;
			if(bDay)
			{
				Mon--;
				Day = MonMap[Mon] + Day;//Day<0
				bDay = false;
			}
			bMon = Mon<1?true:false;
			Year = Time1.tm_year - Time2.tm_year;
			if(bMon)
			{
				Year--;
				Mon = 12 + Mon;//Mon<0;
				bMon = false;
			}
			if(Year < 1900)//C++ time函数获取的是自1900年之后过去了多少年,当计算的年份小于1900时，可能会出现表示问题
			{
				NLOG(LOGNLP,Error,TEXT("Year:%d < 1900"),Year);
				return RelTime;
			}
		}
		RelTime.tm_year = Year;
		RelTime.tm_mon = Mon;
		RelTime.tm_mday = Day;
		RelTime.tm_hour = Hour;
		RelTime.tm_min = Min;
		RelTime.tm_sec = Sec;
		return RelTime;
	}
	// 比较两个std::tm格式的时间的大小，返回-1表示Time1<Time2，返回1表示Time1>Time2,返回0表示Time1=Time2
	inline int TmCompare(const std::tm Time1,const std::tm Time2)
	{
		int Year = Time1.tm_year - Time2.tm_year;
		int Mon = Time1.tm_mon - Time2.tm_mon;
		int Day = Time1.tm_mday - Time2.tm_mday;
		int Hour = Time1.tm_hour - Time2.tm_hour;
		int Min = Time1.tm_min - Time2.tm_min;
		int Sec = Time1.tm_sec - Time2.tm_sec;
		if(Year < 0){return -1;}
		if(Year > 0){return 1;}
		if(Mon < 0){return -1;}
		if(Mon > 0){return 1;}
		if(Day < 0){return -1;}
		if(Day > 0){return 1;}
		if(Hour < 0){return -1;}
		if(Hour > 0){return 1;}
		if(Min < 0){return -1;}
		if(Min > 0){return 1;}
		if(Sec < 0){return -1;}
		if(Sec > 0){return 1;}
		return 0;
	}
	// 时间字符串转Tm，时间字符串格式为：xxxx-xx-xx xx:xx:xx
	inline std::tm StringToTm(const std::string& TimeStr)
	{
		std::tm Time = {0};
		std::vector<std::string> DateAndTimeVec;
		limonp::Split(TimeStr,DateAndTimeVec," ");
		if(DateAndTimeVec.size() == 2)
		{
			std::vector<std::string> DateVec;
			limonp::Split(DateAndTimeVec[0],DateVec,"-");
			if(DateVec.size() == 3)
			{
				Time.tm_year = std::stoi(DateVec[0]);
				Time.tm_mon = std::stoi(DateVec[1]);
				Time.tm_mday = std::stoi(DateVec[2]);
			}
			else
			{
				NLOG(LOGNLP,Error,TEXT("The time format is wrong : %s"),*TOFS(TimeStr));
				return Time;
			}
			std::vector<std::string> TimeVec;
			limonp::Split(DateAndTimeVec[1],TimeVec,":");
			if(TimeVec.size() == 3)
			{
				Time.tm_hour = std::stoi(TimeVec[0]);
				Time.tm_min = std::stoi(TimeVec[1]);
				Time.tm_sec = std::stoi(TimeVec[2]);
				return Time;
			}
			else
			{
				NLOG(LOGNLP,Error,TEXT("The time format is wrong : %s"),*TOFS(TimeStr));
				return Time;
			}
		}
		else
		{
			std::vector<std::string> DateVec;
			limonp::Split(DateAndTimeVec[0],DateVec,"-");
			if(DateVec.size() == 3)
			{
				Time.tm_year = std::stoi(DateVec[0]);
				Time.tm_mon = std::stoi(DateVec[1]);
				Time.tm_mday = std::stoi(DateVec[2]);
				return Time;
			}
			NLOG(LOGNLP,Error,TEXT("The time format is wrong : %s"),*TOFS(TimeStr));
			return Time;
		}
	}
	// Tm时间转字符串，字符串格式:xxxx-xx-xx xx:xx:xx
	inline std::string TmToString(std::tm Time)
	{
		return std::to_string(Time.tm_year)+"-"+std::to_string(Time.tm_mon)+"-"+std::to_string(Time.tm_mday)+" "+
			std::to_string(Time.tm_hour)+":"+std::to_string(Time.tm_min)+":"+std::to_string(Time.tm_sec);
	}
	// 获取本机当前时间
	inline std::tm GetNowLocalTime()
	{
		std::time_t Timet = std::time(NULL);
		std::tm Tm;
		localtime_s(&Tm,&Timet);
		//time函数获取的是自1900年之后过去了多少年，所以计算具体年份时需要加上1900
		Tm.tm_year += 1900;
		//tm结构体中tm_mon的表示范围为[0-11],所以在表示具体月份时许需要加1
		Tm.tm_mon += 1;
		return Tm;
	}
	// 将一句话拆分成一个个的字，支持纯中文、纯英文和中英文混合
	inline std::vector<std::string> SplitTextToWord(const std::string& Text)
	{
		int Num = Text.size();
		int i = 0;
		std::vector<std::string> RelVec;
		while(i < Num)
		{
			int size = 1;
			if(Text[i] & 0x80)
			{
				char Chr = Text[i];
				Chr <<= 1;
				do
				{
					Chr <<= 1;
					++size;
				}
				while (Chr & 0x80);
			}
			std::string Word = Text.substr(i,size);
			RelVec.push_back(Word);
			i += size;
		}
		return RelVec;
	}
	// 字符串数值转整型数值，支持中文字符串和数字字符串
	inline int64 StrValueToIntValue(const std::string NumWord)
	{
		std::map<std::string,int> ChineseNumMap = {
			{"零",0},{"一",1},{"二",2},{"两",2},{"三",3},{"四",4},{"五",5},{"六",6},{"七",7},{"八",8},{"九",9},
			{"十",10},{"百",100},{"千",1000},{"万",10000},{"亿",100000000},
			{"0",0},{"1",1},{"2",2},{"3",3},{"4",4},{"5",5},{"6",6},{"7",7},{"8",8},{"9",9},
		};
		std::vector<std::string> SingleNumVec = GlobalManager::SplitTextToWord(NumWord);
		int64 Rel = 0, Tmp = 0, HndMln = 0, Float = 0;
		int64 CurrDigit;
		std::string Point = "";
		for (std::string Word : SingleNumVec)
		{
			if (ChineseNumMap.find(Word) == ChineseNumMap.end())
			{
				break;
			}
			CurrDigit = ChineseNumMap[Word];
			// 处理亿位数
			if (CurrDigit == std::pow(10, 8))
			{
				Rel += Tmp;
				Rel *= CurrDigit;
				HndMln = HndMln * std::pow(10, 8) + Rel;
				Rel = 0;
				Tmp = 0;
			}
			// 处理万位数
			else if (CurrDigit == pow(10, 4))
			{
				Rel += Tmp;
				Rel *= CurrDigit;
				Tmp = 0;
			}
			// 处理千、百、十位数
			else if (CurrDigit >= 10)
			{
				if (Tmp == 0)
				{
					Tmp = 1;
				}
				Rel = Rel + CurrDigit * Tmp;
				Tmp = 0;
			}
			// 处理个位数
			else
			{
				Tmp = Tmp * 10 + CurrDigit;
			}
		}
		Rel += Tmp;
		Rel += HndMln;
		return Rel;
	}

	inline void OpenDatebase(std::string Path = "Default",sqlite3* DB = nullptr)
	{
		if(Path == "Default")
		{
			Path = RESOURCE_ABSOLUTE_PATH + std::string(ConfigManager::Get().TrainModuleConfig.at("DefaultDatabasePath"));
		}
		if(DB == nullptr)
		{
			int Err = sqlite3_open(Path.c_str(),&GlobalManager::Database);
			if(Err)
			{
				NLOG(LOGNLP,Error,TEXT("[GlobalManager::OpenDatebase]%s"),*TOFS(std::string(sqlite3_errmsg(GlobalManager::Database))));
			}
		}
		else
		{
			int Err = sqlite3_open(Path.c_str(),&DB);
			if(Err)
			{
				NLOG(LOGNLP,Error,TEXT("[GlobalManager::OpenDatebase]%s"),*TOFS(std::string(sqlite3_errmsg(DB))));
			}
		}
	}
	
	inline void CloseDatebase(sqlite3* DB = nullptr)
	{
		if(DB == nullptr)
		{
			if(GlobalManager::Database == nullptr) return;
			int Err = sqlite3_close(GlobalManager::Database);
			if(Err)
			{
				NLOG(LOGNLP,Error,TEXT("[GlobalManager::CloseDatebase]%s"),*TOFS(std::string(sqlite3_errmsg(GlobalManager::Database))));
			}
		}
		else
		{
			int Err = sqlite3_close(DB);
			if(Err)
			{
				NLOG(LOGNLP,Error,TEXT("[GlobalManager::CloseDatebase]%s"),*TOFS(std::string(sqlite3_errmsg(DB))));
			}
		}
	}
	
	inline void CreateTable(const std::string& Name = "statement")
	{
		GlobalManager::DatabaseTableName = Name;
		GlobalManager::OpenDatebase();
		char* ErrMsg;
		int Err;
		std::string SqlDelte = "DROP TABLE IF EXISTS " + DatabaseTableName;
		Err = sqlite3_exec(GlobalManager::Database,SqlDelte.c_str(),nullptr,0,&ErrMsg);
		if(Err != SQLITE_OK)
		{
			NLOG(LOGNLP,Warning,TEXT("[GlobalManager::CreateTable]%s"),*TOFS(std::string(ErrMsg)));
		}
		std::string Sql = "CREATE TABLE IF NOT EXISTS "+ DatabaseTableName + " ("
				"id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
				"text NTEXT	NOT NULL,"
				"search_text NTEXT	NOT NULL,"
				"conversation CHAR(20),"
				"created_at	 DATETIME,"
				"in_response_to	NTEXT NOT NULL);";
		Err = sqlite3_exec(GlobalManager::Database,Sql.c_str(),nullptr,0,&ErrMsg);
		if(Err != SQLITE_OK)
		{
			NLOG(LOGNLP,Warning,TEXT("[GlobalManager::CreateTable]%s"),*TOFS(std::string(ErrMsg)));
		}
	}

	inline void SelectAll(int (*Callback)(void*,int,char**,char**), void * Obj,const std::string& TableName = "")
	{
		char* Sql;
		if(TableName == "")
		{
			Sql = sqlite3_mprintf("SELECT * FROM %q",GlobalManager::DatabaseTableName.c_str());
		}
		else
		{
			Sql = sqlite3_mprintf("SELECT * FROM %q",TableName.c_str());
		}
		char* ErrMsg;
		int Err = sqlite3_exec(Database,Sql,Callback,Obj,&ErrMsg);
		if(Err)
		{
			NLOG(LOGNLP,Error,TEXT("[GlobalManager::SelectAllSearchTextCol]%s"),*TOFS(std::string(sqlite3_errmsg(Database))));
		}
	}

	inline sqlite3* GetDatabase()
	{
		if(GlobalManager::Database == nullptr)
		{
			OpenDatebase();
		}
		return GlobalManager::Database;
	}

}

#endif

