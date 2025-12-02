#include "FBaseLogicAdapter.h"

#include <Format.h>

#include "NLP/Common/GlobalManager.h"
#include "NLP/Managers/FPythonServerManager.h"

FBaseLogicAdapter::FBaseLogicAdapter()
{
	// 在BaseLogicAdapter单例对象生命周期内保持与Sqlite3数据库的连接
	//GlobalManager::OpenDatebase();
	// 加载数据库中的search_text和in_response_to字段存在到内存中
	//LoadQuestionFromSqlite3();
	//FPythonServerManager::Get().Word2VecServerSetup();
	//FPythonServerManager::Get().BaseLogicAdapterCallback = std::bind(&FBaseLogicAdapter::SendNextQuestion,this,std::placeholders::_1);
	NLOG(LOGNLP,Log,TEXT("FBaseLogicAdapter constructed"));
}

std::vector<std::string> FBaseLogicAdapter::GetSimilarQuestions(const std::string& Input)
{
	std::vector<std::string> RelVec;
	//SimilarityQuestionVec.clear();
	//Text = Input;
	//std::string Data = GenerateQuesBatchCmd(0,50000,Input);
	////NLOG(LOGNLP,Log,TEXT("%s"),*TOFS(Data));
	////std::string Data = "{\"Index\":0,\"Source\":\"" + Text + "\",\"Target\":\"" + QuestionCorpus[0].first + "\"}";
	//FPythonServerManager::Get().SocCmd("Word2Vec","Similarity",Data);
	//QuestionCorpusIndex++;
	return RelVec;
}

int FBaseLogicAdapter::Sqlite3ColCallback(void* ThisObj, int Argc, char** Argv, char** AzColName)
{
	/*if(Argc == 6)
	{
		((FBaseLogicAdapter*)ThisObj)->QuestionCorpus.push_back(std::pair<std::string,std::string>(std::string(Argv[2]),std::string(Argv[5])));
	}*/
	return 0;
}

bool FBaseLogicAdapter::Process(const std::string& Input, std::string& Output)
{
	/*FPythonServerManager::Get().SocConnect();
	GetSimilarQuestions(Input);*/
	return true;
}

void FBaseLogicAdapter::LoadQuestionFromSqlite3()
{
	//GlobalManager::SelectAll(&FBaseLogicAdapter::Sqlite3ColCallback,this);
}

FBaseLogicAdapter& FBaseLogicAdapter::Get()
{
	static FBaseLogicAdapter Instance;
	return Instance;
}

FBaseLogicAdapter::~FBaseLogicAdapter()
{
	//GlobalManager::CloseDatebase();
	NLOG(LOGNLP,Log,TEXT("FBaseLogicAdapter destructed"));
}


void FBaseLogicAdapter::SendNextQuestion(const nlohmann::json& Json)
{
	//std::string SendData;
	//float Similarity = std::stof(std::string(Json.at("Data").at("Similarity")));
	//int Tag = Json.at("Data").at("Tag");
	//if(Tag == 10)
	//{
	//	for(QAS Qas : SimilarityQuestionVec)
	//	{
	//		NLOG(LOGNLP,Log,TEXT("[Q]%s\t[A]%s\t[S]%f"),*TOFS(Qas.first.first),*TOFS(Qas.first.second),Qas.second);
	//	}
	//}
	//if(Similarity >= SimilarityThreshold)
	//{
	//	NLOG(LOGNLP,Log,TEXT("[Q]%s\t[A]%s"),*TOFS(QuestionCorpus[QuestionCorpusIndex-1].first),*TOFS(QuestionCorpus[QuestionCorpusIndex-1].second));
	//	SimilarityQuestionVec.push_back(QAS(QuestionCorpus[QuestionCorpusIndex-1],Similarity));
	//}
	//if(QuestionCorpusIndex >= (int64)QuestionCorpus.size())
	//{
	//	SendData = "{\"Index\":-1,\"Source\":\"\",\"Target\":\"\"}";
	//	return;
	//}
	//
	//SendData = "{\"Index\":" + std::to_string(QuestionCorpusIndex) + ",\"Source\":\"" + Text + "\",\"Target\":\"" + QuestionCorpus[QuestionCorpusIndex].first + "\"}";
	//FPythonServerManager::Get().SocCmd("Word2Vec","Similarity",SendData);
	//QuestionCorpusIndex++;
}

std::string FBaseLogicAdapter::GenerateQuesBatchCmd(const long& Start,long End,const std::string& Source)
{
	//if(End >= (long)QuestionCorpus.size())
	//{
	//	NLOG(LOGNLP,Warning,TEXT("Index \"End\"=%ld greater than QuestionCorpus.size()=%ld,so set \"End\" = QuestionCorpus.size()"),End,(long)QuestionCorpus.size());
	//	End = (long)QuestionCorpus.size();
	//}
	//if(QuestionCorpusIndex != Start)
	//{
	//	NLOG(LOGNLP,Warning,TEXT("QuestionCorpusIndex not equel"));
	//	QuestionCorpusIndex = Start;
	//}
	//std::string Target;
	//for(QuestionCorpusIndex;QuestionCorpusIndex<End;QuestionCorpusIndex++)
	//{
	//	Target += util::Format("\"{0}\",",QuestionCorpus[QuestionCorpusIndex].first);
	//}
	//Target.pop_back();
	//Target = "[" + Target + "]";
	////std::string Data = "{\"Index\":-1,\"Source\":\"\",\"Target\":\"\"}";
	//std::string Data = util::Format("\"Start\":{0},\"End\":{1},\"Source\":\"{2}\",\"Target\":{3}",Start,End,Source,Target);
	//return "{" + Data + "}";

	return "";
}
