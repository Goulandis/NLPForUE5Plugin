#include "FPythonServerManager.h"
#include "NLP/Common/GlobalManager.h"

FPythonServerManager& FPythonServerManager::Get()
{
	static FPythonServerManager Instance;
	return Instance;
}

bool FPythonServerManager::SocConnect(const std::string& InIP,const int InPort)
{
	return Soc.SocConnet();
}

bool FPythonServerManager::SocSend(const std::string& Msg)
{
	return Soc.SocSend(Msg);
}

bool FPythonServerManager::SocCmd(const std::string& InCmd, const std::string& InType, const std::string& InData)
{
	return Soc.SocCmd(InCmd,InType,InData);
}

void FPythonServerManager::SocClose()
{
	Soc.SocClose();
}

void FPythonServerManager::Word2VecServerClose()
{
	::SendMessage(Word2VecServerWindwosHandle,WM_CLOSE,0,0);
}

void FPythonServerManager::Word2VecServerSetup()
{
	std::thread Word2VecServerSetupThread([]()
	{
		std::string Cmd = TCHAR_TO_UTF8(*(PROJECTPLUGINDIR + "NLPFORUE/Scripts/Gensim/Setup.bat"));
		system(Cmd.c_str());
	});
	Word2VecServerSetupThread.detach();
}

void FPythonServerManager::Word2VecServerWindowFind()
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

FPythonServerManager::FPythonServerManager()
{
	Soc = FSoc();
}

FPythonServerManager::~FPythonServerManager()
{
}
