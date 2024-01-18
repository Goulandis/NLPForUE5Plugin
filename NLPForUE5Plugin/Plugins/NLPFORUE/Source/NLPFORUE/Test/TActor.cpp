#include "TActor.h"
#include "NLPFORUE/Common/FDefine.h"
#include "NLP/Common/GlobalManager.h"
#include <regex>
#include "Python.h"
#include <codecvt>

DEFINE_LOG_CATEGORY(LOGNLP);

FString ATActor::ComTest(FString Text)
{
	std::string Input = TCHAR_TO_UTF8(*Text);
	std::string Output;

	//MSql->Train();
	//MSql->Handle(Input,Output);
	
	//std::wstring cmd = L"E:/Anaconda/envs/NLPFORUE/Python.exe E:/Goulandis/NLPForUE5Plugin/NLPForUE5Plugin/Plugins/NLPFORUE/Scripts/Gensim/Word2Vec.py 这是测试字符串1 这是测试字符串2";
	//const char* GBK_LOCAL_NAME = "CHS";
	//std::wstring_convert<std::codecvt<wchar_t,char,mbstate_t>> conv(new std::codecvt<wchar_t,char,mbstate_t>(GBK_LOCAL_NAME));
	//std::wstring wcmd = conv.from_bytes(cmd);
	// std::wstring_convert<std::codecvt_utf8<wchar_t>> Converter;
	// std::string utf8cmd = Converter.to_bytes(cmd);
	//
	// system("E:/Anaconda/envs/NLPFORUE/Python.exe E:/Goulandis/NLPForUE5Plugin/NLPForUE5Plugin/Plugins/NLPFORUE/Scripts/Gensim/Word2Vec.py");

	FString Rel = FString(UTF8_TO_TCHAR(Output.c_str()));
	return Rel;
}

bool ATActor::SocConnect(FString InIP, int InPort)
{
	Soc = FSoc();
	return Soc.SocConnet();
}

bool ATActor::SocSend(FString Msg)
{
	std::string MsgStr = TCHAR_TO_UTF8(*Msg);
	return Soc.SocSend(MsgStr);
}

bool ATActor::SocCmd(FString InCmd, FString InType, FString InData)
{
	std::string Cmd = TCHAR_TO_UTF8(*InCmd);
	std::string Type = TCHAR_TO_UTF8(*InType);
	std::string Data = TCHAR_TO_UTF8(*InData);
	return Soc.SocCmd(Cmd,Type,Data);
}

void ATActor::SocClose()
{
	Soc.SocClose();
}

void ATActor::Word2VecServerClose()
{
	::SendMessage(Word2VecServerWindwosHandle,WM_CLOSE,0,0);
}

void ATActor::Word2VecServerSetup()
{
	std::thread Word2VecServerSetupThread([]()
	{
		std::string Cmd = TCHAR_TO_UTF8(*(PROJECTPLUGINDIR + "NLPFORUE/Scripts/Gensim/Setup.bat"));
		system(Cmd.c_str());
	});
	Word2VecServerSetupThread.detach();
}

void ATActor::Word2VecServerWindowFind()
{
	Word2VecServerWindwosHandle = ::FindWindow(NULL,_T("Word2VecServer"));
	if(Word2VecServerWindwosHandle == NULL)
	{
		NLOG(LOGNLP,Log,TEXT("No window Word2VecServer found"));
	}
	else
	{
		NLOG(LOGNLP,Error,TEXT("Window Word2VecServer has been found : %d"),Word2VecServerWindwosHandle);
	}
}

// Sets default values
ATActor::ATActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MPrep = MakeShareable(new FPreprocessorModule());
	MLoap = MakeShareable(new FLogicAdapterModule());
	MSql = MakeShareable(new FSqliteModule());
}

ATActor::~ATActor()
{
}

void ATActor::BeginDestroy()
{
	
	Super::BeginDestroy();
}

// Called when the game starts or when spawned
void ATActor::BeginPlay()
{
	Super::BeginPlay();
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

