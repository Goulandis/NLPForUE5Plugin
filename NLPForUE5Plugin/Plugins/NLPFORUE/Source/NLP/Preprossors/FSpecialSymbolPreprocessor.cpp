// Fill out your copyright notice in the Description page of Project Settings.


#include "FSpecialSymbolPreprocessor.h"

FSpecialSymbolPreprocessor::FSpecialSymbolPreprocessor()
{
	// BYTE Tmp[] = {0x00,0x08,0x0A,0x0D,0x0B,0x20};
	// SpecialSymbols = &Tmp;
}

FSpecialSymbolPreprocessor::~FSpecialSymbolPreprocessor()
{
}

FSpecialSymbolPreprocessor& FSpecialSymbolPreprocessor::CreateInstance()
{
	static FSpecialSymbolPreprocessor Instance;
	return Instance;
}

std::string FSpecialSymbolPreprocessor::DeteleSpecialSymbol(std::string Text)
{
	int Len = Text.size()+1;
	char* TChar = new char[Len];
	strcpy_s(TChar,Len,Text.c_str());
	char* Rel = new char[Len];
	int j = 0;
	for(int i=0;i<Len;++i)
	{
		if(IsSpecialSymbol(TChar[i]))
		{
			continue;
		}
		else
		{
			Rel[j] = TChar[i];
			++j;
		}
	}
	std::string RelStr;
	RelStr += Rel;
	return RelStr;
}

std::string FSpecialSymbolPreprocessor::DeteleSpecialSymbol(FString Text)
{
	std::string Str = TCHAR_TO_UTF8(*Text);
	return DeteleSpecialSymbol(Str);
}

bool FSpecialSymbolPreprocessor::IsSpecialSymbol(char& Chr)
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
