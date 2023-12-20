#include "FSpecialSymbolPreprocessor.h"

std::string FSpecialSymbolPreprocessor::DeleteSpaceSymbol(const std::string& Text)
{
	std::string Rel;
	for(int i=0;i<Text.size();++i)
	{
		if(Text[i] != 0x20)
		{
			Rel += Text[i];
		}
	}
	return Rel;
}

FSpecialSymbolPreprocessor::FSpecialSymbolPreprocessor(){}

FSpecialSymbolPreprocessor::~FSpecialSymbolPreprocessor(){}

FSpecialSymbolPreprocessor& FSpecialSymbolPreprocessor::CreateInstance()
{
	static FSpecialSymbolPreprocessor Instance;
	return Instance;
}

// std::string FSpecialSymbolPreprocessor::DeteleSpecialSymbol(const std::string& Text)
// {
// 	int Len = Text.size()+1;
// 	char* TChar = new char[Len];
// 	strcpy_s(TChar,Len,Text.c_str());
// 	char* Rel = new char[Len];
// 	int j = 0;
// 	for(int i=0;i<Len;++i)
// 	{
// 		if(IsSpecialSymbol(TChar[i]))
// 		{
// 			continue;
// 		}
// 		else
// 		{
// 			Rel[j] = TChar[i];
// 			++j;
// 		}
// 	}
// 	std::string RelStr;
// 	RelStr += Rel;
// 	delete TChar;
// 	delete Rel;
// 	return RelStr;
// }

std::string FSpecialSymbolPreprocessor::DeteleSpecialSymbol(const std::string& Text)
{
	vector<std::string> Words = GlobalManager::SplitTextToWord(Text);
	std::string Rel;
	for(std::string Word : Words)
	{
		if(IsSpecialSymbol(Word))
		{
			continue;
		}
		else
		{
			Rel += Word;
		}
	}
	return Rel;
}

bool FSpecialSymbolPreprocessor::IsSpecialSymbol(const char& Chr)
{
	for(BYTE sm : SpecialSymbols)
	{
		if(Chr == sm)
		{
			return true;
		}
	}
	
	return false;
}

inline bool FSpecialSymbolPreprocessor::IsSpecialSymbol(const std::string Word)
{
	const char* Chr = Word.data();
	for(BYTE sm : SpecialSymbols)
	{
		if(*Chr == sm)
		{
			return true;
		}
	}
	return false;
}
