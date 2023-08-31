#include "FMathLogicAdapter.h"
#include <algorithm>
#include <fstream>
#include "NLP/Common/LogDefine.h"
#include "NLP/Common/GlobalManager.h"
#include "NLP/Managers/FPreprocessorFactory.h"

FMathLogicAdapter::FMathLogicAdapter()
{
	UE_LOG(LOGNLP,Log,TEXT("FMathLogicAdapter constructed"));
}

FMathLogicAdapter& FMathLogicAdapter::CreateInstance()
{
	static FMathLogicAdapter Instance;
	return Instance;
}

FMathLogicAdapter::~FMathLogicAdapter()
{
	UE_LOG(LOGNLP,Log,TEXT("FMathLogicAdapter destructed"));
}

// 处理数学问题
bool FMathLogicAdapter::Process(const std::string& Input,std::string& Output)
{
	std::string Sentence = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FSpecialSymbolPreprocessor>()->DeleteSpaceSymbol(Input);
	std::vector<std::string> Words;
	GlobalManager::jieba.Cut(Sentence,Words);
	bool bContainChinese = GlobalManager::IsChinese(Sentence);
	bool bMatchRegex = GlobalManager::RegexMathFormulas(Sentence);
	bool bContianUnit = ContainUnit(Sentence);
	bool bContainMathWord = ContainMathWord(Sentence);
	
	// 如果问题字符串不包含中文字符，则进入数学算式处理流
	if(!bContainChinese)
	{
		std::vector<OpeAndInd> Oai;
		if(MatchMathRegex(Words,Oai))
		{
			Output = OperationFormula(Words,Oai);
		}
		else
		{
			return false;
		}
	}
	// 如果问题字符串包含中文又匹配数学算式，则进入中文+数学算式处理流
	if(bContainChinese && bMatchRegex)
	{
		
	}
	UE_LOG(LOGNLP,Warning,TEXT("Flag9"));
	return false;
}

bool FMathLogicAdapter::ContainUnit(const std::string& Text)
{
	std::regex rp(R"(百|千|万|亿|兆|毫米|厘米|分米|米|克|千克|吨)");
	for(size_t i=0;i<Text.size();++i)
	{
		if(GlobalManager::IsNumber(&Text[i]))
		{
			
		}
	}
	
	return std::regex_search(Text,rp);
}

bool FMathLogicAdapter::ContainMathWord(const std::string& Text)
{
	return 0;
}

bool FMathLogicAdapter::MatchMathRegex(std::vector<std::string>& Words, std::vector<OpeAndInd>& Oai)
{
	std::string MatchStr = "+-*/^";
	std::vector<std::string> Vec;
	std::regex FormulaMatch(R"([\+\-\*/\^0-9])");
	bool Rel = false;
	for(int i=0;i<Words.size();++i)
	{
		if(std::regex_search(Words[i],FormulaMatch))
		{
			Vec.push_back(Words[i]);
		}
	}
	Words.clear();
	Words = Vec;
	Oai.clear();
	for(int i=0;i<Words.size();++i)
	{
		if(MatchStr.find(Words[i]) != std::string::npos)
		{
			OpeAndInd Tmp;
			Tmp.Ope = Words[i];
			Tmp.Ind = i;
			// 设置算式运算符的运算优先级，^优先级为3，*和/优先级为2，+和—优先级为1
			if(Words[i] == "+" || Words[i] == "-"){Tmp.Ord = 1;}
			else if(Words[i] == "*" || Words[i] == "/"){Tmp.Ord = 2;}
			else if(Words[i] == "^"){Tmp.Ord = 3;}
			Oai.push_back(Tmp);
			Rel = true;
		}
	}
	
	// 按照运算符优先级对vector进行排序
	std::sort(Oai.begin(),Oai.end(),[](const OpeAndInd& Oai1,const OpeAndInd& Oai2){return Oai1.Ord > Oai2.Ord;});
	return Rel;
}

bool FMathLogicAdapter::IsFloat(const std::string& Num)
{
	std::string Point = ".";
	std::string::size_type Ind = Num.find(Point);
	return Ind != std::string::npos;
}

std::string FMathLogicAdapter::FormatFloat(const std::string& Num)
{
	const char* TChar = Num.data();
	std::string Rel;
	while(true)
	{
		if(*TChar == 0)
		{
			break;
		}
		if(*TChar != 0x30)
		{
			Rel += *TChar;
		}
		TChar++;
	}
	return Rel;
}

/* 将多运算符算式拆分为单运算符算式，然后进行递归运算，拆分过程如下：
* 1+1.2+2*4/2^2-2*2
* 2^2=4 -> 1+1.2+2*4/4-2*2
* 2*4=8 -> 1+1.2+8/4-2*2
* 8/4=2 -> 1+1.2+2-2*2
* 2*2=4 -> 1+1.2+2-4
* 1+1.2=2.2 -> 2.2+2-4
* 2.2+2=4.2 -> 4.2-4
* 4.2-4=0.2 -> 0.2
*/
std::string FMathLogicAdapter::OperationFormula(std::vector<std::string> Words,std::vector<OpeAndInd> Oai)
{
	std::string Rel;
	if(Words.size() == 1 && Oai.size() == 0)
	{
		Rel = Words[0];
		return Rel;
	}

	if(Oai.size() >= 1 && Oai[0].Ind - 1 >= 0 && Oai[0].Ind + 1 < Words.size())
	{
		std::string Prefix = Words[Oai[0].Ind - 1];
		std::string Subfix = Words[Oai[0].Ind + 1];
		const char* TChar = Prefix.data();
		std::string RelStr;
		// 如果前后缀数字有一个是小数，则使用小数处理
		if(IsFloat(Prefix) || IsFloat(Subfix))
		{
			float NumPrefix = std::stof(TChar);
			TChar = Subfix.data();
			float NumSubfix = std::stof(TChar);
			std::ostringstream Oss;
			Oss << SingleOperationFormula(NumPrefix,NumSubfix,Oai[0].Ope);
			RelStr = Oss.str();
		}
		// 如果前后缀数字没有一个是小数，则认为都是整数
		else
		{
			int NumPrefix = std::stoi(TChar);
			TChar = Subfix.data();
			int NumSubfix = std::stoi(TChar);
			int Output = SingleOperationFormula(NumPrefix,NumSubfix,Oai[0].Ope);
			RelStr = std::to_string(Output);
		}
		Words[Oai[0].Ind] = RelStr;
		Words.erase(Words.begin()+Oai[0].Ind + 1);
		Words.erase(Words.begin()+Oai[0].Ind - 1);
		// 生成新的数学算式和运算符优先级
		MatchMathRegex(Words,Oai);
		// 递归按照新的运算符优先级计算新生成的数学算式
		Rel = OperationFormula(Words,Oai);
	}
	return Rel;
}

bool FMathLogicAdapter::IsNumber(std::string Word)
{
	return false;
}
