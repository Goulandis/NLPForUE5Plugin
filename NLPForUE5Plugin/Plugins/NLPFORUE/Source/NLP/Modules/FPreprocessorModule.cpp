#include "FPreprocessorModule.h"
#include "NLP/Common/GlobalManager.h"

FPreprocessorModule::FPreprocessorModule()
{
	Prep_LanguageJudgment = &GetPreprocessor<FLanguageJudgmentPreprocessor>();
	Prep_SensitiveWord = &GetPreprocessor<FSensitiveWordPreprocessor>();
	Prep_SpecialSymbol = &GetPreprocessor<FSpecialSymbolPreprocessor>();
	Prep_StopWord = &GetPreprocessor<FStopWordFilteringPreprocessor>();
	
	std::string SensitiveWordPath = ConfigManager::Get().SensitiveWordPreprocessorConfig.at("SensitiveWordPath");
	const std::string SensitiveWordDictPath = GlobalManager::RESOURCE_ABSOLUTE_PATH + SensitiveWordPath;
	NLOG(LOGNLP,Log,TEXT("Loading sensitive word from %s"),*TOFS(SensitiveWordDictPath));
	Prep_SensitiveWord->LoadSensitiveWordDict(SensitiveWordDictPath);
	NLOG(LOGNLP,Log,TEXT("Loaded sensitive word from %s"),*TOFS(SensitiveWordDictPath));
	NLOG(LOGNLP,Log,TEXT("FPreprocessorModule constructed"));
}

FPreprocessorModule::~FPreprocessorModule()
{
	NLOG(LOGNLP,Log,TEXT("FPreprocessorModule destructed"));
}

void FPreprocessorModule::Handle(const std::string& Input,std::string& Output,const SConfig& InConfig)
{
	ModuleConfig(InConfig);
	Output = Input;
	GlobalManager::ELanguageType LType = GlobalManager::ELanguageType::None;
	if(Config.bLanguageJudgment)
	{
		LType = Prep_LanguageJudgment->GetLanguageType(Output);
	}
	switch (LType)
	{
	case GlobalManager::ELanguageType::zh_CN:
		if(Config.bSensitiveWord) Output = Prep_SensitiveWord->SensitiveWordFiltering(Output);
		if(Config.bSpecialSymbol) Output = Prep_SpecialSymbol->DeteleSpecialSymbol(Output);
		if(Config.bStopWord) Output = Prep_StopWord->StopWordFiltering(Output);
		break;;
	case GlobalManager::ELanguageType::zh_HK:
		break;
	case GlobalManager::ELanguageType::en_US:
		break;
	default:
		break;
	}
}

void FPreprocessorModule::ModuleConfig(const SConfig& InConfig)
{
	Config = InConfig;
}
