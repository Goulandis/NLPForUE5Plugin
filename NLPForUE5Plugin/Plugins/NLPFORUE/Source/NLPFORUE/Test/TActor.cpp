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
	FString Rel = DeteleSpecialSymbol(TEXT("这是一个测    试用例	，删除Tab键\n，删除换行符\t，删除制表符"));
	
	UE_LOG(LOGNLPFORUE,Log,TEXT("%s"),*Rel);
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
		DICT_PATH,
		HMM_PATH,
		USER_DICT_PATH,
		IDF_PATH,
		STOP_WORD_PATH
		);
	
}

// Called every frame
void ATActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

