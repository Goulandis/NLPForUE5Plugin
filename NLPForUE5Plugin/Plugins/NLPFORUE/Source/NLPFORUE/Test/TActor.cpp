// Fill out your copyright notice in the Description page of Project Settings.


#include "TActor.h"
#include "NLPFORUE/Common/FDefine.h"
#include "cppjieba/Jieba.hpp"


// Sets default values
ATActor::ATActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATActor::BeginPlay()
{
	Super::BeginPlay();
}

ELanguageType ATActor::GetLanguageType()
{
	FLanguageJudgmentPreprocessor lp = UPreprocessorFactory::CreateInstance()->GetPreprocessor<FLanguageJudgmentPreprocessor>();
	ELanguageType ltype =  lp.GetLanguageType(TEXT("The half-width character is the ASCii code of 0~127"));
	UE_LOG(LOGNLPFORUE,Log,TEXT("语言为(1-zh_CN,2-en_US)：%d"),ltype);
	return ltype;
}

FString ATActor::DeteleSpecialSymbol(FString Text)
{
	FSpecialSymbolPreprocessor ss = UPreprocessorFactory::CreateInstance()->GetPreprocessor<FSpecialSymbolPreprocessor>();
	return FString(UTF8_TO_TCHAR(ss.DeteleSpecialSymbol(Text).c_str()));
}

FString ATActor::CWS(FString Text)
{
	cppjieba::Jieba jieba(
		TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + DICT_PATH)),
		TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + HMM_PATH)),
		TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + USER_DICT_PATH)),
		TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + IDF_PATH)),
		TCHAR_TO_UTF8(*(FPaths::ProjectPluginsDir() + COM_PATH + STOP_WORD_PATH))
		);
	vector<string> words;
	jieba.Cut(TCHAR_TO_UTF8(*Text),words,true);
	FString Rel;
	for(string str : words)
	{
		Rel += TEXT("\"") + FString(UTF8_TO_TCHAR(str.c_str())) + TEXT("\"  ");
	}
	return Rel;
}

TArray<FString> ATActor::StopWordFiltering(FString Text)
{
	FStopWordFilteringPreprocessor sw = UPreprocessorFactory::CreateInstance()->GetPreprocessor<FStopWordFilteringPreprocessor>();
	vector<string> StopWords = sw.StopWordFiltering(TCHAR_TO_UTF8(*Text));
	TArray<FString> Rel;
	for(string tmp : StopWords)
	{
		Rel.Add(FString(UTF8_TO_TCHAR(tmp.c_str())));
	}
	return Rel;
}

// Called every frame
void ATActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

