#include "FMathLogicAdapter.h"
#include <algorithm>
#include <fstream>
#include "NLP/Managers/FPreprocessorFactory.h"

FMathLogicAdapter::FMathLogicAdapter()
{
	NLOG(LOGNLP,Log,TEXT("FMathLogicAdapter constructed"));
	// 加载词典缓存
	const std::string Path = GlobalManager::RESOURCE_ABSOLUTE_PATH + GlobalManager::MATHCONFIDEXELEVEL_DICT_PATH;
	ifstream Ifs(Path.c_str());
	check(Ifs.is_open());
	std::string Line;
	while(getline(Ifs,Line))
	{
		// 行提取
		LineDict.push_back(Line);
		// 关键词提取
		std::vector<cppjieba::KeywordExtractor::Word> TempKeyWords;
		GlobalManager::jieba.extractor.ExtractContainStopWord(Line,TempKeyWords,1);
		if(TempKeyWords.size() != 0)
		{
			KeyDict.insert(TempKeyWords[0].word);
		}
		// 动词、名词、数词提取
		for(std::string Tmp : LineDict)
		{
			vector<std::pair<std::string,std::string>> Tagers;
			GlobalManager::jieba.Tag(Tmp,Tagers);
			for(std::pair<std::string,std::string> Pair : Tagers)
			{
				if(Pair.second == CX_V)
				{
					VerbDict.insert(Pair.first);
					continue;
				}
				if(Pair.second == CX_N)
				{
					NounDict.insert(Pair.first);
				}
			}
		}
	}
	Ifs.close();
}

FMathLogicAdapter& FMathLogicAdapter::CreateInstance()
{
	static FMathLogicAdapter Instance;
	return Instance;
}

FMathLogicAdapter::~FMathLogicAdapter()
{
	NLOG(LOGNLP,Log,TEXT("FMathLogicAdapter destructed"));
}

// 处理数学问题
bool FMathLogicAdapter::Process(const std::string& Input,std::string& Output)
{
	std::string Sentence = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FSpecialSymbolPreprocessor>()->DeleteSpaceSymbol(Input);
	std::vector<std::string> Words;
	GlobalManager::jieba.Cut(Sentence,Words);
	bool bContainChinese = GlobalManager::IsChinese(Sentence);
	bool bMatchRegex = GlobalManager::RegexMathFormulas(Sentence);
	bool bContainMathWord = ContainMathWord(Sentence);
	//bool bContianUnit = ContainUnit(Sentence);

	HandleType Type = HandleType::Formula;
	if(!bContainChinese){Type = HandleType::Formula;}
	else if(bContainChinese && bMatchRegex){Type = HandleType::FormulaAndChinese;}
	else if(bContainChinese && !bMatchRegex && bContainMathWord){Type = HandleType::Chinese;}
	//else if(bContainChinese && !bMatchRegex && bContainMathWord && bContianUnit){Type = HandleType::ChieseWithUnit;}
	
	// 如果问题字符串不包含中文字符，则进入数学算式处理流
	if(Type == HandleType::Formula)
	{
		std::vector<OpeAndInd> Oai;
		if(MatchMathRegex(Words,Oai))
		{
			Output = OperationFormula(Words,Oai);
			return true;
		}
		else
		{
			return false;
		}
	}
	// 如果问题字符串包含中文又匹配数学算式，则进入中文+数学算式处理流
	if(Type == HandleType::FormulaAndChinese)
	{
		// 当置信度大于等于可信度，则认为当前句子是数学问式
		if(ConfideceLevel(Input,Type) >= Confidence)
		{
			std::vector<OpeAndInd> Oai;
			std::vector<std::string> FormulaCut;
			std::string Formula = GetBestMatchFormula(Input,Type);
			GlobalManager::jieba.Cut(Formula,FormulaCut);
			if(MatchMathRegex(FormulaCut,Oai))
			{
				Output = OperationFormula(FormulaCut,Oai);
				return true;
			}
			else
			{
				NLOG(LOGNLP,Error,TEXT("Error formula:%s"),*FString(UTF8_TO_TCHAR(Formula.c_str())));
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	// 如果问题字符串包含中文且不匹配数学算式，则进入纯中文数学问题处理流
	if(Type == HandleType::Chinese)
	{
		if(ConfideceLevel(Input,Type) >= Confidence)
		{
			std::string Formula = GetBestMatchFormula(Input,Type);
			std::vector<std::pair<std::string,int64>> NumPair = ChineseNumToInt(SplitTextToNum(Formula));
			for(std::pair<std::string,int64> Pai : NumPair)
			{
				Formula.replace(Formula.find(Pai.first),Pai.first.length(),std::to_string(Pai.second));
			}
			Output = OperationFormula(Formula);
		}
		
	}
	return false;
}

// bool FMathLogicAdapter::ContainUnit(const std::string& Text)
// {
// 	std::regex Pattern(R"(百|千|万|亿|兆|毫米|厘米|分米|米|克|千克|吨)");
// 	return std::regex_search(Text,Pattern);
// }

bool FMathLogicAdapter::ContainMathWord(const std::string& Text)
{
	std::regex Pattern(MathWord);
	return std::regex_search(Text,Pattern);
}

bool FMathLogicAdapter::MatchMathRegex(std::vector<std::string>& Words, std::vector<OpeAndInd>& Oai)
{
	std::string MatchStr = "+-*/^%√";
	std::vector<std::string> Vec;
	std::regex FormulaMatch(R"([\+\-\*/\^%0-9])");
	std::map<std::string,GlobalManager::OpeTag> OpeMap = {
		{"+",GlobalManager::OpeTag::Add},{"-",GlobalManager::OpeTag::Sub},{"*",GlobalManager::OpeTag::Mul},
		{"/",GlobalManager::OpeTag::Div},{"%",GlobalManager::OpeTag::Rem},{"^",GlobalManager::OpeTag::Pow},
		{"√",GlobalManager::OpeTag::Rot}
	};
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
			//Tmp.Ope = Words[i];
			Tmp.Ope = OpeMap[Words[i]];
			Tmp.Ind = i;
			// 设置算式运算符的运算优先级，^优先级为3，*和/优先级为2，+和—优先级为1
			// if(Words[i] == "+" || Words[i] == "-"){Tmp.Ord = 1;}
			// else if(Words[i] == "*" || Words[i] == "/"){Tmp.Ord = 2;}
			// else if(Words[i] == "^" ){Tmp.Ord = 3;}
			if(Tmp.Ope == GlobalManager::OpeTag::Add || Tmp.Ope == GlobalManager::OpeTag::Sub){Tmp.Ord = 1;}
			else if(Tmp.Ope == GlobalManager::OpeTag::Mul || Tmp.Ope == GlobalManager::OpeTag::Div){Tmp.Ord = 2;}
			else if(Tmp.Ope == GlobalManager::OpeTag::Pow || Tmp.Ope == GlobalManager::OpeTag::Rot){Tmp.Ord = 3;}
			
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

std::string FMathLogicAdapter::Keep2DecimalPlaces(std::string Num)
{
	if(!GlobalManager::IsNumber(Num))
	{
		NLOG(LOGNLP,Error,TEXT("Parameter Num is not number,Num:%s"),*FString(UTF8_TO_TCHAR(Num.c_str())));
		return "";
	}
	std::size_t Pos = Num.find(".");
	std::string Left,Right;
	if(Pos != Num.npos)
	{
		Left = Num.substr(0,Pos);
		Right = Num.substr(Pos+1,Num.size());
	}
	if(std::stoi(Right) == 0)
	{
		return Left;
	}
	std::string Float = Right.substr(0,2);
	return Left+"."+Float;
}

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
		if(IsFloat(Prefix) || IsFloat(Subfix) || Oai[0].Ope == GlobalManager::OpeTag::Rot)
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

int FMathLogicAdapter::ConfideceLevel(const std::string& Text, const HandleType& Type)
{
	std::string LocalText = Text;
	int Level = 0;
	LocalText = MathTextFormat(LocalText,Type);
	// 子句匹配，寻找句子中是否存在与字典中预设问题完全匹配的子句，如果存在则置信度+1，否则-1，如果存在多个子句完全匹配的子句则取匹配度最长的
	{
		std::string MatchRel;
		int MaxLen = 0;
		for(std::string Str : LineDict)
		{
			std::string RelStr = RegexSubStringMatch(LocalText,Str);
			if(RelStr.length() > MaxLen)
			{
				MaxLen = RelStr.length();
				MatchRel = RelStr;
			}
		}
		if(MaxLen != 0)
		{
			Level += 1;
			NLOG(LOGNLP,Log,TEXT("子句匹配成功，置信度+1，子句：%s"),*FString(UTF8_TO_TCHAR(MatchRel.c_str())));
		}
		else
		{
			Level -= 1;
			NLOG(LOGNLP,Log,TEXT("子句匹配失败，置信度-1，子句：%s"),*FString(UTF8_TO_TCHAR(MatchRel.c_str())));
		}
	}
	// 关键词匹配，提取句子中权重最大的关键词，如果关键词存在于关键词词典中，则置信度+1，否者-1
	{
		std::vector<cppjieba::KeywordExtractor::Word> TempKeyWords;
		std::string LocalTextForExtract = KeyExtractClean(LocalText,Type);
		GlobalManager::jieba.extractor.ExtractContainStopWord(LocalTextForExtract,TempKeyWords,1);
		if(TempKeyWords.size() != 0)
		{
			if(KeyDict.find(TempKeyWords[0].word) != KeyDict.end())
			{
				Level += 1;
				NLOG(LOGNLP,Log,TEXT("关键词匹配成功，置信度+1，关键词：%s"),*FString(UTF8_TO_TCHAR(TempKeyWords[0].word.c_str())));
			}
			else
			{
				Level -= 1;
				NLOG(LOGNLP,Log,TEXT("关键词匹配失败，置信度-1，关键词：%s"),*FString(UTF8_TO_TCHAR(TempKeyWords[0].word.c_str())));
			}
		}
	}
	
	// 动词匹配，提取句子中的动词集合，如果动词集合与词典动词集合存在交集则置信度+1，否则+0
	// 名词匹配，提取句子中的名词集合，如果名词集合与词典名词集合存在交集则置信度+1，否则+0
	{
		vector<std::pair<std::string,std::string>> Tagers;
		GlobalManager::jieba.Tag(LocalText,Tagers);
		bool bMatchVerb = false;
		bool bMatchNoun = false;
		//bool bMatchNum = false;
		for(std::pair<std::string,std::string> Pair : Tagers)
		{
			if(Pair.second == "v" && !bMatchVerb)
			{
				if(VerbDict.find(Pair.first) != VerbDict.end())
				{
					Level += 1;
					bMatchVerb = true;
					NLOG(LOGNLP,Log,TEXT("动词匹配成功，置信度+1，动词：%s"),*FString(UTF8_TO_TCHAR(Pair.first.c_str())));
				}
			}
			else if(Pair.second == "n" && !bMatchNoun)
			{
				if(NounDict.find(Pair.first) != NounDict.end())
				{
					Level += 1;
					bMatchNoun = true;
					NLOG(LOGNLP,Log,TEXT("名词匹配成功，置信度+1，名词：%s"),*FString(UTF8_TO_TCHAR(Pair.first.c_str())));
				}
			}
		}
	}
	
	// 完全匹配，将替换后的句子与字典中的问式匹配，如果存在完全匹配项则置信度+2，否则+0
	for (std::string Str : LineDict)
	{
		if (Str == LocalText)
		{
			Level += 2;
			NLOG(LOGNLP,Log,TEXT("完全匹配成功，置信度+2，匹配句子：%s"),*FString(UTF8_TO_TCHAR(Str.c_str())));
			break;
		}
	}
	
	// 最长子句匹配，寻找句子在字典中的最长子句，如果最长子句在字典中存在完全匹配项则置信度+0，否者-2
	{
		std::string MaxSubString;
		std::string LocalTextForExtract = KeyExtractClean(LocalText,Type);
		for(std::string Str : LineDict)
		{
			std::string Tmp = GetMaxSubString(LocalTextForExtract,Str);
			if(Tmp.find("X") == string::npos)
			{
				continue;
			}
			if(MaxSubString.length() < Tmp.length())
			{
				MaxSubString = Tmp;
			}
		}
		Level -= 2;
		int Sub = 0;
		for(std::string Str : LineDict)
		{
			if(Str == MaxSubString)
			{
				Level += 2;
				Sub += 2;
				break;
			}
		}
		if(Sub == 0)
		{
			NLOG(LOGNLP,Log,TEXT("最长子句匹配失败，置信度-2，最长匹配句子：%s"),*FString(UTF8_TO_TCHAR(MaxSubString.c_str())));
		}
		else
		{
			NLOG(LOGNLP,Log,TEXT("最长子句匹配成功，置信度+0，最长匹配句子：%s"),*FString(UTF8_TO_TCHAR(MaxSubString.c_str())));
		}
	}
	return Level;	
}

std::string FMathLogicAdapter::RegexSubStringMatch(const std::string& Text,const std::string& MatchStr)
{
	std::regex Pattern(MatchStr);
	std::smatch MatchRel;
	std::regex_search(Text,MatchRel,Pattern);
	if(MatchRel.length() != 0)
	{
		int MaxLen = 0;
		std::string Rel;
		for(std::string Tmp : MatchRel)
		{
			if(Tmp.length() > MaxLen)
			{
				MaxLen = Tmp.length();
				Rel = Tmp;
			}
		}
		return Rel;
	}
	return "";
}

std::string FMathLogicAdapter::GetMaxSubString(const std::string& Str1, const std::string& Str2)
{
	int Len1 = Str1.length();
	int Len2 = Str2.length();
	std::vector<std::vector<int>> dp(Len1+1,std::vector<int>(Len2+1,0));
	int MaxLen = 0;
	int EndIndex = 0;
	for(int i=1;i<=Len1;++i)
	{
		for(int j=1;j<=Len2;++j)
		{
			if(Str1[i-1] == Str2[j-1])
			{
				dp[i][j] = dp[i-1][j-1]+1;
				if(dp[i][j] > MaxLen)
				{
					MaxLen = dp[i][j];
					EndIndex = i-1;
				}
			}
			else
			{
				dp[i][j] = 0;
			}
		}
	}
	if(MaxLen == 0)
	{
		return "";
	}
	return Str1.substr(EndIndex-MaxLen+1,MaxLen);
}

std::vector<std::string> FMathLogicAdapter::GetMatchFormulas(std::string Text)
{
	std::vector<std::string> FormulaVec;
	// 正则表达式规则说明：
	// [\p{Han}]*：匹配任意汉字
	// \s*：匹配任意数量的空格符
	// (\d+\s*[\+\-\*/^]\s*) ：匹配任意运算符中间可间隔任意空格符
	// \d+：匹配任意多个的数字组合
	std::regex pattern(R"([\p{Han}]*\s*(\d+\s*[\+\-\*/^]\s*)+\d+\s*[\p{Han}]*)");
	// 在文本中搜索匹配项
	std::smatch Match;
	 while (std::regex_search(Text, Match, pattern))
	 {
	 	FormulaVec.push_back(Match.str());
	 	// 更新文本以继续搜索
	 	Text = Match.suffix(); 
	 }
	return FormulaVec;
}

std::string FMathLogicAdapter::GetBestMatchFormula(const std::string& Text,const HandleType& Type)
{
	std::string MatchRel;
	if(Type == HandleType::FormulaAndChinese)
	{
		int MaxLen = 0;
		std::vector<std::string> FormulaVec = GetMatchFormulas(Text);
		for(std::string Formula : FormulaVec)
		{
			std::string LocalText = Text;
			LocalText.replace(LocalText.find(Formula),Formula.length(),ReplaceWord);
			for(std::string Line : LineDict)
			{
				std::string RelStr = RegexSubStringMatch(LocalText,Line);
				if(RelStr.length() >= MaxLen)
				{
					MaxLen = RelStr.length();
					MatchRel = Formula;
				}
			}
		}
	}
	else if(Type == HandleType::Chinese)
	{
		std::vector<std::string> Words;
		GlobalManager::jieba.Cut(Text,Words,false);
		std::regex Pattren("(加|减|乘|除|加上|减去|乘以|除以|平方|二次幂|立方|三次幂|次方|平方根|立方根|和|的和|之和|差|的差|之差|"
					 "积|的积|之积|商|的商|之商|对|求余|求的余数|开方|的|与)");
		std::string Tmp;
		for(int i=0;i<Words.size();++i)
		{
			if(GlobalManager::IsNumber(Words[i]) || std::regex_search(Words[i],Pattren))
			{
				Tmp += Words[i];
				if(i == Words.size() - 1)
				{
					MatchRel = Tmp;
				}
			}
			else
			{
				if(Tmp != "")
				{
					MatchRel = Tmp;
				}
				Tmp = "";
			}
		}
	}
	return MatchRel;
}

std::string FMathLogicAdapter::MathTextFormat(std::string Text,const HandleType Type)
{
	if(Type == HandleType::FormulaAndChinese)
	{
		std::string MatchStr = GetBestMatchFormula(Text,Type);
		if(MatchStr != "")
		{
			Text.replace(Text.rfind(MatchStr),MatchStr.length(),ReplaceWord);
			return Text;
		}
		return "";
	}
	else if(Type == HandleType::Chinese)
	{
		std::string ReplaceSource = GetBestMatchFormula(Text,Type);
		Text.replace(Text.find(ReplaceSource),ReplaceSource.length(),ReplaceWord);
		return Text;
	}
	return "";
}

std::string FMathLogicAdapter::FindFormulaDescription(const std::string& Text,std::vector<std::string>* FormulaVec)
{
	std::vector<std::string> Words;
	GlobalManager::jieba.Cut(Text,Words);
	std::regex Pattren("(加|减|乘|除|加上|减去|乘以|除以|平方|立方|次方|平方根|立方根|和|的和|之和|差|的差|之差|积|的积|之积|商|的商|之商|求余|开方)");
	std::string Formula,Tmp;
	for(std::string Word:Words)
	{
		if(GlobalManager::IsNumber(Word) || std::regex_search(Word,Pattren))
		{
			Tmp += Word;
		}
		else
		{
			if(std::regex_search(Tmp,Pattren))
			{
				Formula = Tmp;
				if(FormulaVec != nullptr)
				{
					FormulaVec->push_back(Formula);
				}
			}
			Tmp = "";
		}
	}
	return Formula;
}

std::vector<std::string> FMathLogicAdapter::SplitTextToNum(const std::string& FormulaDescription)
{
	std::vector<std::string> WordVec = GlobalManager::SplitTextToWord(FormulaDescription);
	std::vector<std::string> NumVec;
	std::string NumStr;
	for(int i=0;i<WordVec.size();++i)
	{
		if(GlobalManager::IsNumber(WordVec[i]) || WordVec[i] == "点")
		{
			NumStr += WordVec[i];
			if(i == WordVec.size() - 1)
			{
				NumVec.push_back(NumStr);
				NumStr = "";
			}
		}
		else
		{
			if(NumStr != "")
			{
				NumVec.push_back(NumStr);
				NumStr = "";
			}
		}
	}
	return NumVec;
}

std::string FMathLogicAdapter::OperationFormula(std::string FormulaDescription)
{
	std::vector<std::string> PrePattrens = {
		R"([-]?\d+加[-]?\d+的和|\d+加[-]?\d+之和|\d+加上[-]?\d+的和|\d+加上[-]?\d+之和|\d+和[-]?\d+的和|\d+和[-]?\d+之和|\d+与[-]?\d+的和|\d+与[-]?\d+之和)",
		R"([-]?\d+减[-]?\d+的差|\d+减[-]?\d+之差|\d+减去[-]?\d+的差|\d+减去[-]?\d+之差|\d+和[-]?\d+的差|\d+和[-]?\d+之差|\d+与[-]?\d+的差|\d+与[-]?\d+之差)",
		R"([-]?\d+乘[-]?\d+的积|\d+乘[-]?\d+之积|\d+乘以[-]?\d+的积|\d+乘以[-]?\d+之积|\d+和[-]?\d+的积|\d+和[-]?\d+之积|\d+与[-]?\d+的积|\d+与[-]?\d+之积)",
		R"([-]?\d+除[-]?\d+的商|\d+除[-]?\d+之商|\d+除以[-]?\d+的商|\d+除以[-]?\d+之商|\d+和[-]?\d+的商|\d+和[-]?\d+之商|\d+与[-]?\d+的商|\d+与[-]?\d+之商)",
		R"([-]?\d+的平方根)",
		R"([-]?\d+的立方根)",
		R"([-]?\d+的平方|\d+的二次幂)",
		R"([-]?\d+的立方|\d+的三次幂)",
		R"([-]?\d+对[-]?\d+求的余数)"
	};
	std::vector<std::string> OrdPattrens = {
		R"([-]?\d+加[-]?\d+|\d+加上[-]?\d+)",
		R"([-]?\d+减[-]?\d+|\d+减去[-]?\d+)",
		R"([-]?\d+乘[-]?\d+|\d+乘以[-]?\d+)",
		R"([-]?\d+除[-]?\d+|\d+除以[-]?\d+)",
		R"([-]?\d+对[-]?\d+求余|\d+对[-]?\d+求余数)"
	};

	// 优先级计算，先计算满足可优先计算的算式，并将计算结果替换掉对应的文字描述
	for(int i=0;i<PrePattrens.size();++i)
	{
		std::regex Pattren(PrePattrens[i]);
		std::smatch Matchs;
		std::regex_search(FormulaDescription,Matchs,Pattren);
		if(Matchs.size() > 0)
		{
			for(std::string Formula : Matchs)
			{
				std::vector<std::string> Cut;
				GlobalManager::jieba.Cut(Formula,Cut);
				std::string Prefix="",Subfix="";
				const char* TChar;
				for (int j=0;j<Cut.size();++j)
				{
					if (GlobalManager::IsNumber(Cut[j]))
					{
						if (Prefix == "")
						{
							Prefix = Cut[j];
							// 适配负数
							if(j-1>=0)
							{
								if(Cut[j-1] == "-")
								{
									Prefix = "-"+Prefix;
								}
							}
						}
						else if (Subfix == "")
						{
							Subfix = Cut[j];
							if(j-1>=0)
							{
								if(Cut[j-1] == "-")
								{
									Subfix = "-"+Subfix;
								}
							}
						}
					}
				}
				if(Prefix != "" && Subfix != "")
				{
					switch (i)
					{
					case 0:
						{
							TChar = Prefix.data();
							int PrefixNum = std::stoi(TChar);
							TChar = Subfix.data();
							int SubfixNum = std::stoi(TChar);
							int RelNum = SingleOperationFormula(PrefixNum,SubfixNum,GlobalManager::OpeTag::Add);
							FormulaDescription.replace(FormulaDescription.find(Formula),Formula.length(),std::to_string(RelNum));
							NLOG(LOGNLP,Log,TEXT("计算加法，算式:%s"),*FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					case 1:
						{
							TChar = Prefix.data();
							int PrefixNum = std::stoi(TChar);
							TChar = Subfix.data();
							int SubfixNum = std::stoi(TChar);
							int RelNum = SingleOperationFormula(PrefixNum,SubfixNum,GlobalManager::OpeTag::Sub);
							FormulaDescription.replace(FormulaDescription.find(Formula),Formula.length(),std::to_string(RelNum));
							NLOG(LOGNLP,Log,TEXT("计算减法，算式:%s"),*FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					case 2:
						{
							TChar = Prefix.data();
							int PrefixNum = std::stoi(TChar);
							TChar = Subfix.data();
							int SubfixNum = std::stoi(TChar);
							int RelNum = SingleOperationFormula(PrefixNum,SubfixNum,GlobalManager::OpeTag::Mul);
							FormulaDescription.replace(FormulaDescription.find(Formula),Formula.length(),std::to_string(RelNum));
							NLOG(LOGNLP,Log,TEXT("计算乘法，算式:%s"),*FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					case 3:
						{
							TChar = Prefix.data();
							float PrefixNum = std::stof(TChar);
							TChar = Subfix.data();
							float SubfixNum = std::stof(TChar);
							float RelNum = SingleOperationFormula(PrefixNum,SubfixNum,GlobalManager::OpeTag::Div);
							FormulaDescription.replace(FormulaDescription.find(Formula),Formula.length(),std::to_string(RelNum));
							NLOG(LOGNLP,Log,TEXT("计算除法，算式:%s"),*FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					case 8:
						{
							TChar = Prefix.data();
							int PrefixNum = std::stoi(TChar);
							TChar = Subfix.data();
							int SubfixNum = std::stoi(TChar);
							int RelNum = SingleOperationFormula(PrefixNum,SubfixNum,GlobalManager::OpeTag::Rem);
							FormulaDescription.replace(FormulaDescription.find(Formula),Formula.length(),std::to_string(RelNum));
							NLOG(LOGNLP,Log,TEXT("计算求余，算式:%s"),*FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					}
				}
				else if(Prefix != "" && Subfix == "")
				{
					switch (i)
					{
					case 4:
						{
							TChar = Prefix.data();
							float SubfixNum = std::stof(TChar);
							float PrefixNum = 2;
							float RelNum = SingleOperationFormula(PrefixNum, SubfixNum, GlobalManager::OpeTag::Rot);
							std::string FormatNum = FormatFloat(std::to_string(RelNum));
							std::string NumStr = Keep2DecimalPlaces(std::to_string(RelNum));
							FormulaDescription.replace(FormulaDescription.find(Formula), Formula.length(),NumStr);
							NLOG(LOGNLP, Log, TEXT("计算平方根，算式:%s"), *FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					case 5:
						{
							TChar = Prefix.data();
							float SubfixNum = std::stof(TChar);
							float PrefixNum = 3;
							float RelNum = SingleOperationFormula(PrefixNum, SubfixNum, GlobalManager::OpeTag::Rot);
							std::string NumStr = Keep2DecimalPlaces(std::to_string(RelNum));
							FormulaDescription.replace(FormulaDescription.find(Formula), Formula.length(),NumStr);
							NLOG(LOGNLP, Log, TEXT("计算立方根，算式:%s"), *FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					case 6:
						{
							TChar = Prefix.data();
							int PrefixNum = std::stoi(TChar);
							int SubfixNum = 2;
							int RelNum = SingleOperationFormula(PrefixNum,SubfixNum,GlobalManager::OpeTag::Pow);
							FormulaDescription.replace(FormulaDescription.find(Formula),Formula.length(),std::to_string(RelNum));
							NLOG(LOGNLP,Log,TEXT("计算平方，算式:%s"),*FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					case 7:
						{
							TChar = Prefix.data();
							int PrefixNum = std::stoi(TChar);
							int SubfixNum = 3;
							int RelNum = SingleOperationFormula(PrefixNum,SubfixNum,GlobalManager::OpeTag::Pow);
							FormulaDescription.replace(FormulaDescription.find(Formula),Formula.length(),std::to_string(RelNum));
							NLOG(LOGNLP,Log,TEXT("计算立方，算式:%s"),*FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					}
				}
			}
		}
	}

	// 剩下的只能顺序计算的描述，直接进行顺序计算，并将计算结果替换掉对应的文字描述
	std::regex Pattren(R"([-]?\d+加[-]?\d+|[-]?\d+加上[-]?\d+|[-]?\d+减[-]?\d+|[-]?\d+减去[-]?\d+|[-]?\d+乘[-]?\d+|[-]?\d+乘以[-]?\d+|[-]?\d+除[-]?\d+|[-]?\d+除以[-]?\d+|[-]?\d+对[-]?\d+求余|[-]?\d+对[-]?\d+求余数)");
	std::smatch Matchs;
	while (std::regex_search(FormulaDescription, Matchs, Pattren))
	{
		if (Matchs.size() <= 0)
		{
			break;
		}
		std::string Formula = Matchs[0];
		for (int i = 0; i < OrdPattrens.size(); ++i)
		{
			std::regex FormulaPattren(OrdPattrens[i]);
			if (std::regex_search(Formula, FormulaPattren))
			{
				std::vector<std::string> Cut;
				GlobalManager::jieba.Cut(Formula, Cut);
				std::string Prefix = "", Subfix = "";
				const char* TChar;
				for (int j=0;j<Cut.size();++j)
				{
					if (GlobalManager::IsNumber(Cut[j]))
					{
						if (Prefix == "")
						{
							Prefix = Cut[j];
							if(j-1>=0)
							{
								if(Cut[j-1] == "-")
								{
									Prefix = "-"+Prefix;
								}
							}
						}
						else if (Subfix == "")
						{
							Subfix = Cut[j];
							if(j-1>=0)
							{
								if(Cut[j-1] == "-")
								{
									Subfix = "-"+Subfix;
								}
							}
						}
					}
				}
				if (Prefix != "" && Subfix != "")
				{
					switch (i)
					{
					case 0:
						{
							TChar = Prefix.data();
							int PrefixNum = std::stoi(TChar);
							TChar = Subfix.data();
							int SubfixNum = std::stoi(TChar);
							int RelNum = SingleOperationFormula(PrefixNum, SubfixNum, GlobalManager::OpeTag::Add);
							FormulaDescription.replace(FormulaDescription.find(Formula), Formula.length(),
							                           std::to_string(RelNum));
							NLOG(LOGNLP, Log, TEXT("计算加法，算式:%s"), *FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					case 1:
						{
							TChar = Prefix.data();
							int PrefixNum = std::stoi(TChar);
							TChar = Subfix.data();
							int SubfixNum = std::stoi(TChar);
							int RelNum = SingleOperationFormula(PrefixNum, SubfixNum, GlobalManager::OpeTag::Sub);
							FormulaDescription.replace(FormulaDescription.find(Formula), Formula.length(),
							                           std::to_string(RelNum));
							NLOG(LOGNLP, Log, TEXT("计算减法，算式:%s"), *FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					case 2:
						{
							TChar = Prefix.data();
							int PrefixNum = std::stoi(TChar);
							TChar = Subfix.data();
							int SubfixNum = std::stoi(TChar);
							int RelNum = SingleOperationFormula(PrefixNum, SubfixNum, GlobalManager::OpeTag::Mul);
							FormulaDescription.replace(FormulaDescription.find(Formula), Formula.length(),
							                           std::to_string(RelNum));
							NLOG(LOGNLP, Log, TEXT("计算乘法，算式:%s"), *FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					case 3:
						{
							TChar = Prefix.data();
							float PrefixNum = std::stof(TChar);
							TChar = Subfix.data();
							float SubfixNum = std::stof(TChar);
							float RelNum = SingleOperationFormula(PrefixNum, SubfixNum, GlobalManager::OpeTag::Div);
							FormulaDescription.replace(FormulaDescription.find(Formula), Formula.length(),
							                           std::to_string(RelNum));
							NLOG(LOGNLP, Log, TEXT("计算除法，算式:%s"), *FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					case 4:
						{
							TChar = Prefix.data();
							int PrefixNum = std::stoi(TChar);
							TChar = Subfix.data();
							int SubfixNum = std::stoi(TChar);
							int RelNum = SingleOperationFormula(PrefixNum, SubfixNum, GlobalManager::OpeTag::Rem);
							FormulaDescription.replace(FormulaDescription.find(Formula), Formula.length(),
							                           std::to_string(RelNum));
							NLOG(LOGNLP, Log, TEXT("计算求余，算式:%s"), *FString(UTF8_TO_TCHAR(Formula.c_str())));
						}
						break;
					}
				}
			}
		}
	}
	// 排除中文算式描述提取时，部分提取词对最终答案的干扰
	if(GlobalManager::IsChinese(FormulaDescription))
	{
		std::vector<std::string> Words;
		GlobalManager::jieba.Cut(FormulaDescription,Words);
		for(std::string Item : Words)
		{
			if(GlobalManager::IsNumber(Item))
			{
				FormulaDescription = Item;
			}
		}
	}
	return FormulaDescription;
}

std::vector<std::pair<std::string, int64>> FMathLogicAdapter::ChineseNumToInt(const std::vector<std::string>& NumVec)
{
	std::map<std::string,int> ChineseNumMap = {
		{"零",0},{"一",1},{"二",2},{"两",2},{"三",3},{"四",4},{"五",5},{"六",6},{"七",7},{"八",8},{"九",9},
		{"十",10},{"百",100},{"千",1000},{"万",10000},{"亿",100000000},
		{"0",0},{"1",1},{"2",2},{"3",3},{"4",4},{"5",5},{"6",6},{"7",7},{"8",8},{"9",9},
	};
	std::vector<std::pair<std::string,int64>> PaiVec;
	for(std::string Item : NumVec)
	{
		std::vector<std::string> SingleNumVec = GlobalManager::SplitTextToWord(Item);
		int64 Rel = 0,Tmp=0,HndMln=0,Float=0;
		int64 CurrDigit;
		std::string Point = "";
		for(std::string Word : SingleNumVec)
		{
			if(ChineseNumMap.find(Word) == ChineseNumMap.end())
			{
				break;
			}
			CurrDigit = ChineseNumMap[Word];
			// 处理亿位数
			if(CurrDigit == std::pow(10,8))
			{
				Rel += Tmp;
				Rel *= CurrDigit;
				HndMln = HndMln * std::pow(10,8) + Rel;
				Rel = 0;
				Tmp = 0;
			}
			// 处理万位数
			else if(CurrDigit == pow(10,4))
			{
				Rel += Tmp;
				Rel *= CurrDigit;
				Tmp = 0;
			}
			// 处理千、百、十位数
			else if(CurrDigit >= 10)
			{
				if(Tmp == 0)
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
		PaiVec.push_back(std::pair(Item,Rel));
	}
	return PaiVec;
}

std::string FMathLogicAdapter::KeyExtractClean(std::string Text,HandleType Type)
{
	if(Type == HandleType::FormulaAndChinese)
	{
		std::vector<std::string> WordVec;
		GlobalManager::jieba.Cut(Text,WordVec);
		std::string Formula = "";
		for(std::string Word : WordVec)
		{
			if(!GlobalManager::IsChinese(Word))
			{
				Formula += Word;
			}
			else
			{
				if(Formula != "")
				{
					Text.replace(Text.find(Formula),Formula.length(),ReplaceWord);
					Formula = "";
				}
			}
		}
	}
	else if(Type == HandleType::Chinese)
	{
		std::vector<std::string> FormulaVec;
		FindFormulaDescription(Text,&FormulaVec);
		for(std::string FormulaDescription : FormulaVec)
		{
			Text.replace(Text.find(FormulaDescription),FormulaDescription.length(),ReplaceWord);
		}
		std::vector<std::string> WordVec;
		GlobalManager::jieba.Cut(Text,WordVec);
		std::string ReplaceStr = "";
		for(std::string Word : WordVec)
		{
			if(GlobalManager::IsNumber(Word))
			{
				ReplaceStr += Word;
			}
			else
			{
				if(ReplaceStr != "")
				{
					Text.replace(Text.find(ReplaceStr),ReplaceStr.length(),ReplaceWord);
					ReplaceStr = "";
				}
			}
		}
	}
	return Text;
}
