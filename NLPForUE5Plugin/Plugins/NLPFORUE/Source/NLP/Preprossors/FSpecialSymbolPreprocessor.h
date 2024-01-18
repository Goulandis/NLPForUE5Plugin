#pragma once
#include "FPreprocessor.h"
#include "CoreMinimal.h"

class NLP_API FSpecialSymbolPreprocessor : public FPreprocessor
{
public:
	static FSpecialSymbolPreprocessor& Get();
	~FSpecialSymbolPreprocessor();

	std::string DeleteSpaceSymbol(const std::string& Text);
	//std::string DeteleSpecialSymbol(const std::string& Text);
	std::string DeteleSpecialSymbol(const std::string& Text);
	bool IsSpecialSymbol(const char& Chr);
	bool IsSpecialSymbol(const std::string Word);
private:
	FSpecialSymbolPreprocessor();

	/*
	 * 0x00:空字符	0x08:退格符	0x09:水平制表符
	 * 0x0A:换行符	0x0B:垂直制表符	0x0D:回车键
	 * 0x20:空格
	 */
	BYTE SpecialSymbols[7] = {0x00,0x08,0x09,0x0A,0x0B,0x0D,0x20};
	std::vector<std::string> SpecialSymbolVec;
};
