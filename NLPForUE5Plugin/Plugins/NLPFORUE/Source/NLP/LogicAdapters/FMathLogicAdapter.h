#pragma once
#include <regex>
#include <vector>
#include <set>
#include <string>
#include <math.h>
#include "FLogicAdapter.h"
#include "CoreMinimal.h"
#include "NLP/Common/LogDefine.h"

struct OpeAndInd
{
	std::string Ope;
	int Ind;
	int Ord;
};

// 数学逻辑适配器，处理数学相关的问题
class NLP_API FMathLogicAdapter : public FLogicAdapter
{
public:
	static FMathLogicAdapter& CreateInstance(); 
	
	~FMathLogicAdapter();
	
	bool Process(const std::string& Input,std::string& Output);
	bool ContainUnit(const std::string& Text);
	// 判断全中文数学问题字符串中是否包含数学运算词
	bool ContainMathWord(const std::string& Text);
	bool MatchMathRegex(std::vector<std::string>& Words,std::vector<OpeAndInd>& Oai);
	bool IsFloat(const std::string& Num);
	std::string FormatFloat(const std::string& Num);
	std::string OperationFormula(std::vector<std::string> Words,std::vector<OpeAndInd> Oai);
	
	std::string GetMathPattrn(){return MathPattern;}

	bool IsNumber(std::string Word);

private:
	FMathLogicAdapter();

	template<typename T>
	T SingleOperationFormula(const T& Prefix,const T& Subfix,const std::string Operation)
	{
		T Rel;
		if(Operation == "+"){Rel = Prefix + Subfix;}
		else if(Operation == "-"){Rel = Prefix - Subfix;}
		else if(Operation == "*"){Rel = Prefix * Subfix;}
		else if(Operation == "/"){Rel = Prefix / Subfix;}
		else if(Operation == "^"){Rel = pow(Prefix,Subfix);}
		return Rel;
	}

	std::string MathPattern;
	std::set<std::string> OpeSet;
	std::vector<std::regex> RegexVec;
};


