#include "TActor.h"
#include "NLPFORUE/Common/FDefine.h"
#include "NLP/Common/LogDefine.h"
#include "NLP/Managers/FLogicAdapterFactory.h"
#include "NLP/Common/GlobalManager.h"
#include <regex>

DEFINE_LOG_CATEGORY(LOGNLP);

// Sets default values
ATActor::ATActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

ELanguageType ATActor::GetLanguageType()
{
	FLanguageJudgmentPreprocessor* lp = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FLanguageJudgmentPreprocessor>();
	string temp = TCHAR_TO_UTF8(TEXT("The half-width character is the ASCii code of 0~127"));
	ELanguageType ltype =  lp->GetLanguageType(temp);
	UE_LOG(LOGNLPFORUE,Log,TEXT("语言为(1-zh_CN,2-en_US)：%d"),ltype);
	return ltype;
}

FString ATActor::CWS(FString Text)
{
	vector<string> words;
	GlobalManager::jieba.Cut(TCHAR_TO_UTF8(*Text),words,true);
	FString Rel;
	for(string str : words)
	{
		Rel += TEXT("\"") + FString(UTF8_TO_TCHAR(str.c_str())) + TEXT("\"  ");
	}
	return Rel;
}

TArray<FString> ATActor::StopWordFiltering(FString Text)
{
	FStopWordFilteringPreprocessor* sw = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FStopWordFilteringPreprocessor>();
	vector<string> StopWords = sw->StopWordFiltering(TCHAR_TO_UTF8(*Text));
	TArray<FString> Rel;
	for(string tmp : StopWords)
	{
		Rel.Add(FString(UTF8_TO_TCHAR(tmp.c_str())));
	}
	return Rel;
}

FString ATActor::SensitiveWordFiltering(FString Text)
{
	FString DictPath = FPaths::ProjectPluginsDir()+TEXT("NLPFORUE/Resources/textfilter/keywords");
	FSensitiveWordPreprocessor* sw = FPreprocessorFactory::CreateInstance()->GetPreprocessor<FSensitiveWordPreprocessor>();
	sw->LoadSensitiveWordDict(TCHAR_TO_UTF8(*DictPath));
	string Rel = sw->SensitiveWordFiltering(TCHAR_TO_UTF8(*Text));
	return FString(UTF8_TO_TCHAR(Rel.c_str()));
}

FString ATActor::PreprocessorModuleTest(FString Text)
{
	MPrep = MakeShareable(new FPreprocessorModule());
	FString Rel;
	for(FString Str : MPrep->Handle(Text))
	{
		Rel += TEXT("\'") + Str + TEXT("\' ");
	}
	return Rel;
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
	const size_t Topk = 5;
	vector<cppjieba::KeywordExtractor::Word> KeyWords;
	string Str = TCHAR_TO_UTF8(*Text);
	GlobalManager::jieba.extractor.Extract(Str,KeyWords,Topk);
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

bool ATActor::RegexTest(FString Text)
{
	std::vector<std::string> Words;
	GlobalManager::jieba.Cut(TCHAR_TO_UTF8(*Text),Words);
	//针对数学算术式子的正则匹配
	{
		//char* Chr = Words[0];
		//std::isdigit(Words[0]);//传入char
		std::regex rp(R"((\d+\s*[\+\-\*/\^x]\s*)+\d+)");
		std::string input = TCHAR_TO_UTF8(*Text);
		return std::regex_search(input,rp);
	}
}

FString ATActor::ComTest(FString Text)
{
	FMathLogicAdapter* mla = FLogicAdapterFactory::CreateInstance()->GetLogicAdapter<FMathLogicAdapter>();
	std::string Input = TCHAR_TO_UTF8(*Text);
	std::string Output;
	if(mla->Process(Input,Output))
	{
		return FString(UTF8_TO_TCHAR(Output.c_str()));
	}
	else
	{
		UE_LOG(LOGNLP,Error,TEXT("Process False"));
		return "";
	}
}

// Called every frame
void ATActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

