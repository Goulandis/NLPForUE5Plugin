#pragma once
#define WIN32_LEAN_AND_MEAN
#include "WinSock2.h"
#include "string"
#include <functional>
#include "NLP/Common/GlobalManager.h"
#include "CoreMinimal.h"

#pragma comment(lib,"ws2_32.lib")

class NLP_API FSoc
{
public:
	FSoc();
	~FSoc();

	bool SocConnet(const std::string& InIP = "127.0.0.1",const int InPort = 7214);
	bool SocReconnet();
	bool SocSend(const std::string& Msg);
	bool SocCmd(const std::string& Cmd, const std::string& Type, const std::string& Data);
	void SocClose();
	void OnRecv(std::string Msg,const FSoc* Soc);
	static void Recv(const FSoc* Soc);

	bool IsConnected = false;
	std::function<void(nlohmann::json)> ExternalCallbacks;
private:
	SOCKADDR_IN AddrSrv;
	SOCKET SocClient = 0;
	std::function<void(std::string,const FSoc* Soc)> OnRecvCallback;
	static std::string MsgBuff;
};
