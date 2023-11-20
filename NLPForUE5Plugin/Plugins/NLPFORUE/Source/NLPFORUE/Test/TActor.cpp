#include "TActor.h"
#include "NLPFORUE/Common/FDefine.h"
#include "NLP/Common/LogDefine.h"
#include "NLP/Common/GlobalManager.h"
#include <regex>

DEFINE_LOG_CATEGORY(LOGNLP);

FString ATActor::ComTest(FString Text)
{
	std::string Input = TCHAR_TO_UTF8(*Text);
	std::string Output;

	//std::regex P(R"(\d+年\b(?:[1-9]|1[0-2])\b月\b(?:[1-9]|1\d|2[0-9]|3[0-1])\b日|\d+年\b(?:[1-9]|1[0-2])\b月\b(?:[1-9]|1\d|2[0-9]|3[0-1])\b号)");
	//std::regex P(R"([零一二三四五六七八九]+年(?:[一二三四五六七八九]|十[一二])月(?:[一二三四五六七八九]|十[一二三四五六七八九]|二十[零一二三四五六七八九]|三十[一])日|[零一二三四五六七八九]+年(?:[一二三四五六七八九]|十[一二])月(?:[一二三四五六七八九]|十[一二三四五六七八九]|二十[零一二三四五六七八九]|三十[一])号)");
	std::regex P(R"([零一二三四五六七八九]+年[零一二三四五六七八九十]+月[零一二三四五六七八九十]+日|[零一二三四五六七八九]+年[零一二三四五六七八九十]+月[零一二三四五六七八九十]+号)");
	std::smatch Matchs;
	std::regex_search(Input,Matchs,P);
	if(Matchs.size() > 0)
	{
		Output = Matchs[0];
	}
	FString Rel = FString(UTF8_TO_TCHAR(Output.c_str()));
	return Rel;
}

// Sets default values
ATActor::ATActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mla = FLogicAdapterFactory::CreateInstance()->GetLogicAdapter<FMathLogicAdapter>();
	wla = FLogicAdapterFactory::CreateInstance()->GetLogicAdapter<FWeatherLogicAdapter>();
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

bool ATActor::RegexTest(FString Text)
{

	//针对数学算术式子的正则匹配
	std::regex rp(R"((\d+\s*[\+\-\*/\^x]\s*)+\d+)");
	std::string input = TCHAR_TO_UTF8(*Text);
	std::smatch sm;
	bool Rel = std::regex_search(input, sm, rp);
	for (std::string t : sm)
	{
		UE_LOG(LOGNLP, Log, TEXT("匹配的算式：%s"), *FString(UTF8_TO_TCHAR(t.c_str())));
	}

	return Rel;
}

bool ATActor::IsNumber(FString NumStr)
{
	std::string Str = TCHAR_TO_UTF8(*NumStr);
	return GlobalManager::IsNumber(Str);
}

FString ATActor::MathLogicAdapter(FString Question)
{
	std::string Input = TCHAR_TO_UTF8(*Question);
	std::string Output;
	mla->Process(Input,Output);
	return FString(UTF8_TO_TCHAR(Output.c_str()));
}

// Called every frame
void ATActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

