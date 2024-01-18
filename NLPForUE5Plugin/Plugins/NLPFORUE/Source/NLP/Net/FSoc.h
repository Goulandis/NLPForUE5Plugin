#pragma once
#define WIN32_LEAN_AND_MEAN
#include "WinSock2.h"
#include "string"
#include <functional>
#include "CoreMinimal.h"

#pragma comment(lib,"ws2_32.lib")

class NLP_API FSoc
{
public:
	FSoc();
	~FSoc();

	bool SocConnet(const std::string& InIP = "127.0.0.1",const int InPort = 7214);
	bool SocSend(const std::string& Msg);
	bool SocCmd(const std::string& Cmd, const std::string& Type, const std::string& Data);
	void SocClose();
	void OnRecv(const std::string Msg);
	static void Recv(const FSoc* Soc);

	bool IsConnected = false;
private:
	SOCKET SocClient = 0;
	std::function<void(const std::string)> OnRecvCallback;
};
