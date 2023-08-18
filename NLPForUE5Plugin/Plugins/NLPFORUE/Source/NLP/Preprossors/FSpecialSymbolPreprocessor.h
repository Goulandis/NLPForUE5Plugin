// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "FPreprocessor.h"
#include "CoreMinimal.h"

/**
 * 
 */
class NLP_API FSpecialSymbolPreprocessor : public FPreprocessor
{
public:
	~FSpecialSymbolPreprocessor();
	static FSpecialSymbolPreprocessor& CreateInstance();

	std::string DeteleSpecialSymbol(std::string Text);
	std::string DeteleSpecialSymbol(FString Text);
	bool IsSpecialSymbol(char& Chr);
private:
	FSpecialSymbolPreprocessor();

	/*
	 * 0x00:空字符	0x08:退格符	0x09:水平制表符
	 * 0x0A:换行符	0x0B:垂直制表符	0x0D:回车键
	 * 0x20:空格
	 */
	BYTE SpecialSymbols[7] = {0x00,0x08,0x09,0x0A,0x0B,0x0D,0x20};
};