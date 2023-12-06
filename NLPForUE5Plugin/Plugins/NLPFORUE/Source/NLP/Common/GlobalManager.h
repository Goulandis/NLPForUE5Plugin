﻿#ifndef GLOBALMANAGER
#define GLOBALMANAGER

#include "cppjieba/Jieba.hpp"
#include "NLP/Preprossors/Define.h"
//#include "NLP/LogicAdapters/Define.h"
#include "UEManager.h"
#include <regex>
#include <time.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "cpp-httplib/httplib.h"

// Global debug functions
namespace DebugLog
{
	
	inline void PrintVec(const std::vector<std::string>& Vec,std::string Flag)
	{
		for (int i=0;i<Vec.size();++i)
		{
			UE_LOG(LogTemp,Log,TEXT("%s:%s"),*FString(UTF8_TO_TCHAR(Flag.c_str())),*FString(UTF8_TO_TCHAR(Vec[i].c_str())));
		}
	}

	inline FString Log(const std::string& Str)
	{
		return FString(UTF8_TO_TCHAR(Str.c_str()));
	}
}

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

	// 逻辑适配器配置文件相对路径
	const char* const LOGICADAPTER_CONFIG_PATH = "Config/LogicAdapter.json";
	// 数学置信度文件相对路径
	const char* const MATHCONFIDEXELEVEL_DICT_PATH = "Dicts/MathConfideceLevel";
	// 中国城市编码列表文件相对路径
	const char* const CITYADCODEXLSX_PATH = "Dicts/CityAdcode";

	
	// 结巴分词库全局实例
	inline cppjieba::Jieba jieba(
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + SOURCE_PATH + DICT_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + SOURCE_PATH + HMM_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + SOURCE_PATH + USER_DICT_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + SOURCE_PATH + IDF_PATH)),
	TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + SOURCE_PATH + STOP_WORD_PATH))
	);

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
				UE_LOG(LOGNLP,Error,TEXT("Year:%d < 1900"),Year);
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
				UE_LOG(LOGNLP,Error,TEXT("The time format is wrong : %s"),*DebugLog::Log(TimeStr));
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
				UE_LOG(LOGNLP,Error,TEXT("The time format is wrong : %s"),*DebugLog::Log(TimeStr));
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
			UE_LOG(LOGNLP,Error,TEXT("The time format is wrong : %s"),*DebugLog::Log(TimeStr));
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

	// inline  Json::Value ReadJsonFromString(const std::string& Str)
	// {
	// 	// Json读对象创建工厂
	// 	Json::CharReaderBuilder ReaderBuilder;
	// 	// 支持UTF8，以支持中文读取
	// 	ReaderBuilder["emitUTF8"] = true;
	// 	// 通过工程创建Json读对象
	// 	std::unique_ptr<Json::CharReader> Reader(ReaderBuilder.newCharReader());
	// 	// Json值对象
	// 	Json::Value Root;
	// 	// 错误信息
	// 	std::string Error;
	// 	// 从字符串中读取Json，并存储到Json值对象中
	// 	bool IsOk = Reader->parse(Str.c_str(),Str.c_str()+Str.size(),&Root,&Error);
	// 	if(!IsOk || !Error.empty())
	// 	{
	// 		UE_LOG(LOGNLP,Error,TEXT("There was an error in json parsing,Error:%s"),*TOFSTR(Error));
	// 	}
	// 	return Root;
	// }
}

#endif

