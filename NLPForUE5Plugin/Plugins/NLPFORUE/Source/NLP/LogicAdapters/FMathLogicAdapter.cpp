#include "FMathLogicAdapter.h"
#include <algorithm>
#include <fstream>
#include "NLP/Common/LogDefine.h"
#include "NLP/Common/GlobalManager.h"
#include "NLP/Managers/FPreprocessorFactory.h"

FMathLogicAdapter::FMathLogicAdapter()
{
	UE_LOG(LOGNLP,Log,TEXT("FMathLogicAdapter constructed"));
	// 加载词典缓存
	const std::string Path = GlobalManager::ResourcePath + MATHCONFIDEXELEVEL_DICT_PATH;
	ifstream Ifs(Path.c_str());
	check(Ifs.is_open())
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
				if(Pair.second == "v")
				{
					VerbDict.insert(Pair.first);
					continue;
				}
				if(Pair.second == "n")
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
	bool bContainMathWord = ContainMathWord(Sentence);
	bool bContianUnit = ContainUnit(Sentence);

	HandleType Type = HandleType::Formula;
	if(!bContainChinese){Type = HandleType::Formula;}
	else if(bContainChinese && bMatchRegex){Type = HandleType::FormulaAndChinese;}
	else if(bContainChinese && !bMatchRegex && bContainMathWord){Type = HandleType::Chinese;}
	else if(bContainChinese && !bMatchRegex && bContainMathWord && bContianUnit){Type = HandleType::ChieseWithUnit;}
	
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
				UE_LOG(LOGNLP,Error,TEXT("Error formula:%s"),*FString(UTF8_TO_TCHAR(Formula.c_str())));
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
			UE_LOG(LOGNLP,Error,TEXT("formula:%s"),*FString(UTF8_TO_TCHAR(Formula.c_str())));
			
			
		}
		
	}
	if(Type == HandleType::ChieseWithUnit)
	{
		
	}
	// 如果问题字符串包含中文且不匹配数学算式，但包含十、百、千、万、亿、兆等计量词，则进入包含计量词的中文
	UE_LOG(LOGNLP,Warning,TEXT("Flag9"));
	return false;
}

bool FMathLogicAdapter::ContainUnit(const std::string& Text)
{
	std::regex Pattern(R"(百|千|万|亿|兆|毫米|厘米|分米|米|克|千克|吨)");
	for(size_t i=0;i<Text.size();++i)
	{
		if(GlobalManager::IsNumber(&Text[i]))
		{
			
		}
	}
	
	return std::regex_search(Text,Pattern);
}

bool FMathLogicAdapter::ContainMathWord(const std::string& Text)
{
	std::regex Pattern(MathWord);
	return std::regex_search(Text,Pattern);
}

bool FMathLogicAdapter::MatchMathRegex(std::vector<std::string>& Words, std::vector<OpeAndInd>& Oai)
{
	std::string MatchStr = "+-*/^%";
	std::vector<std::string> Vec;
	std::regex FormulaMatch(R"([\+\-\*/\^%0-9])");
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
			UE_LOG(LOGNLP,Log,TEXT("子句匹配成功，置信度+1，子句：%s"),*FString(UTF8_TO_TCHAR(MatchRel.c_str())));
		}
		else
		{
			Level -= 1;
			UE_LOG(LOGNLP,Log,TEXT("子句匹配失败，置信度-1，子句：%s"),*FString(UTF8_TO_TCHAR(MatchRel.c_str())));
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
				UE_LOG(LOGNLP,Log,TEXT("关键词匹配成功，置信度+1，关键词：%s"),*FString(UTF8_TO_TCHAR(TempKeyWords[0].word.c_str())));
			}
			else
			{
				Level -= 1;
				UE_LOG(LOGNLP,Log,TEXT("关键词匹配失败，置信度-1，关键词：%s"),*FString(UTF8_TO_TCHAR(TempKeyWords[0].word.c_str())));
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
		bool bMatchNum = false;
		for(std::pair<std::string,std::string> Pair : Tagers)
		{
			if(Pair.second == "v" && !bMatchVerb)
			{
				if(VerbDict.find(Pair.first) != VerbDict.end())
				{
					Level += 1;
					bMatchVerb = true;
					UE_LOG(LOGNLP,Log,TEXT("动词匹配成功，置信度+1，动词：%s"),*FString(UTF8_TO_TCHAR(Pair.first.c_str())));
				}
			}
			else if(Pair.second == "n" && !bMatchNoun)
			{
				if(NounDict.find(Pair.first) != NounDict.end())
				{
					Level += 1;
					bMatchNoun = true;
					UE_LOG(LOGNLP,Log,TEXT("名词匹配成功，置信度+1，名词：%s"),*FString(UTF8_TO_TCHAR(Pair.first.c_str())));
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
			UE_LOG(LOGNLP,Log,TEXT("完全匹配成功，置信度+2，匹配句子：%s"),*FString(UTF8_TO_TCHAR(Str.c_str())));
			break;
		}
	}
	
	// 最长子句匹配，寻找句子在字典中的最长子句，如果最长子句在字典中存在完全匹配项则置信度+0，否者-2
	{
		std::string MaxSubString;
		for(std::string Str : LineDict)
		{
			std::string Tmp = GetMaxSubString(LocalText,Str);
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
			UE_LOG(LOGNLP,Log,TEXT("最长子句匹配失败，置信度-2，最长匹配句子：%s"),*FString(UTF8_TO_TCHAR(MaxSubString.c_str())));
		}
		else
		{
			UE_LOG(LOGNLP,Log,TEXT("最长子句匹配成功，置信度+0，最长匹配句子：%s"),*FString(UTF8_TO_TCHAR(MaxSubString.c_str())));
		}
	}
	UE_LOG(LOGNLP,Log,TEXT("数学置信度:%d"),Level);
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
		GlobalManager::jieba.Cut(Text,Words);
		std::regex Pattren("(加|减|乘|除|加上|减去|乘以|除以|平方|立方|次方|平方根|立方根|和|的和|之和|差|的差|之差|积|的积|之积|商|的商|之商|求余|开方)");
		std::string Tmp;
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
			//UE_LOG(LOGNLP,Log,TEXT("Tmp:%s"),*FString(UTF8_TO_TCHAR(Tmp.c_str())));
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

std::vector<std::string> FMathLogicAdapter::SplitTextToWord(const std::string& Text)
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

std::vector<std::string> FMathLogicAdapter::SplitTextToNum(const std::string& FormulaDescription)
{
	std::vector<std::string> WordVec = SplitTextToWord(FormulaDescription);
	std::vector<std::string> NumVec;
	std::string NumStr;
	for(int i=0;i<WordVec.size();++i)
	{
		if(GlobalManager::IsNumber(WordVec[i]))
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

std::string FMathLogicAdapter::FormulaDescToMathFormula(const std::string& FormulaDescription)
{
	// R"([\p{Han}]*\s*(\d+\s*[\+\-\*/^]\s*)+\d+\s*[\p{Han}]*)"
	std::regex AddPattren(R"([零一二三四五六七八九十百千万亿]+(加|加上|和|与)[零一二三四五六七八九十百千万亿]+[(的和|之和|和)])");
	
	return "";
}

std::vector<std::pair<std::string, int64>> FMathLogicAdapter::ChineseNumToInt(std::vector<std::string> NumVec)
{
	std::map<std::string,int> ChineseNumMap = {
		{"零",0},{"一",1},{"二",2},{"两",2},{"三",3},{"四",4},{"五",5},{"六",6},{"七",7},{"八",8},{"九",9},
		{"十",10},{"百",100},{"千",1000},{"万",10000},{"亿",100000000},
		{"0",0},{"1",1},{"2",2},{"3",3},{"4",4},{"5",5},{"6",6},{"7",7},{"8",8},{"9",9},
	};
	std::vector<std::pair<std::string,int64>> PaiVec;
	for(std::string Item : NumVec)
	{
		std::vector<std::string> SingleNumVec = SplitTextToWord(Item);
		int64 Rel = 0,Tmp=0,HndMln=0;
		
		int64 CurrDigit;
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
