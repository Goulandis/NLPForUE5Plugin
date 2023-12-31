#include "TActor.h"
#include "NLPFORUE/Common/FDefine.h"
#include "NLP/Common/GlobalManager.h"
#include <regex>
#include "../SQLite3/sqlite3.h"

DEFINE_LOG_CATEGORY(LOGNLP);

FString ATActor::ComTest(FString Text)
{
	std::string Input = TCHAR_TO_UTF8(*Text);
	std::string Output;

	// std::string Tmp;
	// MPrep->Handle(Input,Tmp,FPreprocessorModule::SConfig(true,true,true,false));
	// MLoap->Handle(Tmp,Output);

	sqlite3* db;
	const char* dbname = "SQLite3/test.db";
	std::string tmp = TOUTF8(*(PROJECTPLUGINDIR + GlobalManager::RESOURCE_PATH + dbname));
	const char* DBPath = tmp.c_str();
	sqlite3_open(DBPath,&db);

	FString Rel = FString(UTF8_TO_TCHAR(Output.c_str()));
	return Rel;
}

// Sets default values
ATActor::ATActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MPrep = MakeShareable(new FPreprocessorModule());
	MLoap = MakeShareable(new FLogicAdapterModule());
}

ATActor::~ATActor()
{
	
}

void ATActor::BeginDestroy()
{
	//FPreprocessorFactory::CreateInstance()->DestroyInstance();
	//FLogicAdapterFactory::CreateInstance()->DestroyInstance();
	Super::BeginDestroy();
}

// Called when the game starts or when spawned
void ATActor::BeginPlay()
{
	Super::BeginPlay();
}

GlobalManager::ELanguageType ATActor::GetLanguageType()
{
	FLanguageJudgmentPreprocessor* lp = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FLanguageJudgmentPreprocessor>();
	string temp = TCHAR_TO_UTF8(TEXT("The half-width character is the ASCii code of 0~127"));
	GlobalManager::ELanguageType ltype =  lp->GetLanguageType(temp);
	UE_LOG(LOGNLPFORUE,Log,TEXT("语言为(1-zh_CN,2-en_US)：%d"),ltype);
	return ltype;
}

FString ATActor::CWS(FString Text)
{
	// 一加一的和乘以二的积对二求余加二的和开方
	// 1+1 -> 2*2 -> 4%2 -> 2+2 -> sqrt(4)
	// 加、乘以、求余、加、开方
	vector<string> words;
	FString Rel;
	GlobalManager::jieba.Cut(TCHAR_TO_UTF8(*Text),words,true);
	for(string str : words)
	{
		Rel += TEXT("\"") + FString(UTF8_TO_TCHAR(str.c_str())) + TEXT("\"  ");
	}
	UE_LOG(LOGNLP,Log,TEXT("CutT:%s"),*Rel);
	Rel = "";
	words.clear();
	GlobalManager::jieba.Cut(TCHAR_TO_UTF8(*Text),words,false);
	for(string str : words)
	{
		Rel += TEXT("\"") + FString(UTF8_TO_TCHAR(str.c_str())) + TEXT("\"  ");
	}
	UE_LOG(LOGNLP,Log,TEXT("CutF:%s"),*Rel);
	Rel = "";
	words.clear();
	GlobalManager::jieba.CutAll(TCHAR_TO_UTF8(*Text),words);
	for(string str : words)
	{
		Rel += TEXT("\"") + FString(UTF8_TO_TCHAR(str.c_str())) + TEXT("\"  ");
	}
	UE_LOG(LOGNLP,Log,TEXT("CutAll:%s"),*Rel);
	Rel = "";
	words.clear();
	GlobalManager::jieba.CutSmall(TCHAR_TO_UTF8(*Text),words,2);
	for(string str : words)
	{
		Rel += TEXT("\"") + FString(UTF8_TO_TCHAR(str.c_str())) + TEXT("\"  ");
	}
	UE_LOG(LOGNLP,Log,TEXT("CutSmall:%s"),*Rel);
	Rel = "";
	words.clear();
	GlobalManager::jieba.CutHMM(TCHAR_TO_UTF8(*Text),words);
	for(string str : words)
	{
		Rel += TEXT("\"") + FString(UTF8_TO_TCHAR(str.c_str())) + TEXT("\"  ");
	}
	UE_LOG(LOGNLP,Log,TEXT("CutHMM:%s"),*Rel);
	Rel = "";
	words.clear();
	GlobalManager::jieba.CutForSearch(TCHAR_TO_UTF8(*Text),words);
	for(string str : words)
	{
		Rel += TEXT("\"") + FString(UTF8_TO_TCHAR(str.c_str())) + TEXT("\"  ");
	}
	UE_LOG(LOGNLP,Log,TEXT("CutForSearchT:%s"),*Rel);
	Rel = "";
	words.clear();
	GlobalManager::jieba.CutForSearch(TCHAR_TO_UTF8(*Text),words,false);
	for(string str : words)
	{
		Rel += TEXT("\"") + FString(UTF8_TO_TCHAR(str.c_str())) + TEXT("\"  ");
	}
	UE_LOG(LOGNLP,Log,TEXT("CutForSearchF:%s"),*Rel);
	return "";
}


FString ATActor::SensitiveWordFiltering(FString Text)
{
	FString DictPath = FPaths::ProjectPluginsDir()+TEXT("NLPFORUE/Resources/textfilter/keywords");
	FSensitiveWordPreprocessor* sw = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FSensitiveWordPreprocessor>();
	sw->LoadSensitiveWordDict(TCHAR_TO_UTF8(*DictPath));
	string Rel = sw->SensitiveWordFiltering(TCHAR_TO_UTF8(*Text));
	return FString(UTF8_TO_TCHAR(Rel.c_str()));
}


FString ATActor::TestJiebaTag(FString Text)
{
	vector<std::pair<string,string>> Tagers;
	string Str = TCHAR_TO_UTF8(*Text);
	GlobalManager::jieba.Tag(Str,Tagers);
	string RelTmp;
	for(std::pair<string,string> Pa : Tagers)
	{
		RelTmp += Pa.first + ":" + Pa.second + ", ";
	}
	return FString(UTF8_TO_TCHAR(RelTmp.c_str()));
}

FString ATActor::TestJiebaExtract(FString Text)
{
	const size_t Topk = 1;
	vector<cppjieba::KeywordExtractor::Word> KeyWords;
	string Str = TCHAR_TO_UTF8(*Text);
	GlobalManager::jieba.extractor.ExtractContainStopWord(Str,KeyWords,Topk);
	string Rel;
	for(cppjieba::KeywordExtractor::Word KeyWord : KeyWords)
	{
		string Tmp = "[";
		for(size_t t : KeyWord.offsets)
		{
			Tmp += std::to_string(t) + ",";
		}
		Tmp += "]";
		Rel += "word:" + KeyWord.word + ",offset:" + Tmp + ",weight:" + std::to_string(KeyWord.weight);
	}
	return FString(UTF8_TO_TCHAR(Rel.c_str()));
}

void ATActor::PreprocessorTest(FString Text)
{
	std::string Input = TCHAR_TO_UTF8(*Text);
	std::string Output;
	Output = MPrep->Prep_SensitiveWord->SensitiveWordFiltering(Input);
	NLOG(LOGNLP,Log,TEXT("After Sensitive Word:%s"),*TOFS(Output));
	std::string Tmp = Output;
	Output = MPrep->Prep_SpecialSymbol->DeteleSpecialSymbol(Tmp);
	NLOG(LOGNLP,Log,TEXT("After Special Symbol:%s"),*TOFS(Output));
	Tmp = Output;
	Output = MPrep->Prep_StopWord->StopWordFiltering(Tmp);
	NLOG(LOGNLP,Log,TEXT("After Stop Word:%s"),*TOFS(Output));
}

FString ATActor::PrepAndLoapConformity(FString Text)
{
	std::string Input = TCHAR_TO_UTF8(*Text);
	std::string Tmp,Output;
	MPrep->Handle(Input,Tmp,FPreprocessorModule::SConfig(true,true,true,false));
	MLoap->Handle(Tmp,Output);
	return TOFS(Output);
}

// Called every frame
void ATActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

