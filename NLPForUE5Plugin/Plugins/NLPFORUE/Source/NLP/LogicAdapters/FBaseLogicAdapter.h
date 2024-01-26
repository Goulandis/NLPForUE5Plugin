#pragma once
#include "FLogicAdapter.h"
#include "CoreMinimal.h"

// 基础逻辑适配器，处理闲聊相关的问题
class NLP_API FBaseLogicAdapter : public FLogicAdapter
{
public:
	static FBaseLogicAdapter& Get();
	~FBaseLogicAdapter();
	
	bool Process(const std::string& Input,std::string& Output);
	void LoadQuestionFromSqlite3();
private:
	typedef std::pair<std::string,std::string> QAPair;
	typedef std::pair<QAPair,float> QAS;
	
	FBaseLogicAdapter();
	//FBaseLogicAdapter(const FBaseLogicAdapter& Single) = delete;
	//const FBaseLogicAdapter &operator=(const FBaseLogicAdapter& Single) = delete;

	std::vector<std::string> GetSimilarQuestions(const std::string& Input);

	void SendNextQuestion(const nlohmann::json& Json);
	std::string GenerateQuesBatchCmd(const long& Start,long End,const std::string& Source);
	static int Sqlite3ColCallback(void* ThisObj,int Argc,char** Argv,char** AzColName);

	std::vector<QAPair> QuestionCorpus;
	int64 QuestionCorpusIndex = 0;
	std::string Text;
	float SimilarityThreshold = 0.7;
	std::vector<QAS> SimilarityQuestionVec;
};
