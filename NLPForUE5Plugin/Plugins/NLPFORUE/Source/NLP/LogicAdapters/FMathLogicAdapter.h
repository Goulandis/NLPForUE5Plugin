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
	// 判断是否句子是否包含单位词
	bool ContainUnit(const std::string& Text);
	// 判断全中文数学问题字符串中是否包含数学运算词
	bool ContainMathWord(const std::string& Text);
	// 匹配数学算式并返回计算优先级
	bool MatchMathRegex(std::vector<std::string>& Words,std::vector<OpeAndInd>& Oai);
	// 判单数字是否是小数
	bool IsFloat(const std::string& Num);
	// 格式化小数，删除小数末尾多余的0
	std::string FormatFloat(const std::string& Num);
	
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
	std::string OperationFormula(std::vector<std::string> Words,std::vector<OpeAndInd> Oai);
	
	/*
	* 计算数学问题置信度
	* 第一步：使用代词X替换句子中数学算式或中文描述的数学算式子句
	* 第二步：子句匹配，寻找句子中是否存在与字典中预设问题完全匹配的子句，如果存在则置信度+1，否则-1，如果存在多个子句完全匹配的子句则取匹配度最长的
	* 第三步：关键词匹配，提取句子中权重最大的关键词，如果关键词存在于关键词词典中，则置信度+1，否者-1
	* 第四步：动词匹配，提取句子中的动词集合，如果动词集合与词典动词集合存在交集则置信度+1，否则+0
	* 第五步：名词匹配，提取句子中的名词集合，如果名词集合与词典名词集合存在交集则置信度+1，否则+0
	* 第六步：完全匹配，将替换后的句子与字典中的问式匹配，如果存在完全匹配项则置信度+2，否则+0
	* 第七步：最长子句匹配，寻找句子在字典中的最长子句，如果最长子句在字典中存在完全匹配项则置信度+0，否者-2
	*/
	int ConfideceLevel(const std::string& Text,const std::vector<std::string>& Words,std::string& Formula);
	
	std::string RegexSubStringMatch(const std::string& Text,const std::string& MatchStr);
	// 基于动态规划的最大字串截取
	std::string GetMaxSubString(const std::string& Str1,const std::string& Str2);
	// 获取一个句子中所有匹配的数学算式
	std::vector<std::string> GetMatchFormulas(std::string Text);
	// 获取最佳匹配度的数学算式
	std::string GetBestMatchFormula(const std::string& Text,const std::vector<std::string>& LineDict,const std::vector<std::string>& FormulaVec);

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

	const int Confidence = 3;
};