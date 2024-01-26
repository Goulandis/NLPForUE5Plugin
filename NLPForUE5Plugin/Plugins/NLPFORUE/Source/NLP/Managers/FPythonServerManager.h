#pragma once
#include "NLP/Net/FSoc.h"
#include "CoreMinimal.h"

class NLP_API FPythonServerManager
{
public:
	static FPythonServerManager& Get();
	
	bool SocConnect(const std::string& InIP = "127.0.0.1",const int InPort = 7214);
	bool SocReconnect();
	bool SocSend(const std::string& Msg);
	bool SocCmd(const std::string& InCmd, const std::string& InType, const std::string& InData);
	void SocClose();
	void Word2VecServerClose();
	void Word2VecServerSetup();
	void Word2VecServerWindowFind();
	void OnRecvCallback(nlohmann::json Json);

	FSoc Soc;
	std::function<void(const nlohmann::json& Json)> BaseLogicAdapterCallback;
private:
	FPythonServerManager();
	~FPythonServerManager();
	FPythonServerManager(const FPythonServerManager& Single) = delete;
	const FPythonServerManager &operator=(const FPythonServerManager& Single) = delete;
	
	HWND Word2VecServerWindwosHandle;
};
