#include "FSoc.h"

#include <json.hpp>
#include <thread>
#include "Chaos/ChaosPerfTest.h"
#include "NLP/Common/GlobalManager.h"
#include "StringFormat/Format.h"

FSoc::FSoc()
{
	OnRecvCallback = std::bind(&FSoc::OnRecv,this,std::placeholders::_1);
}

FSoc::~FSoc()
{
	closesocket(SocClient);
	WSACleanup();
}

bool FSoc::SocConnet(const std::string& InIP, const int32 InPort)
{
	if(SocClient != 0)
	{
		NLOG(LOGNLP,Log,TEXT("Socket is exists : %d"),SocClient);
		return true;
	}
	SOCKADDR_IN AddrSrv;
	AddrSrv.sin_family = AF_INET;
	AddrSrv.sin_port = htons(InPort);
	AddrSrv.sin_addr.S_un.S_addr = inet_addr(InIP.c_str());

	SocClient = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(SocClient == SOCKET_ERROR)
	{
		NLOG(LOGNLP,Log,TEXT("There was an error with the socket creation : %d"),WSAGetLastError());
		return false;
	}
	if(connect(SocClient,(struct sockaddr*)&AddrSrv,sizeof(AddrSrv)) == INVALID_SOCKET)
	{
		NLOG(LOGNLP,Error,TEXT("There was an error with the socket connection : %d"),WSAGetLastError());
		return false;
	}
	NLOG(LOGNLP,Log,TEXT("[Socket connected]%lld"),SocClient);
	IsConnected = true;
	std::thread RecvThread(Recv,this);
	RecvThread.detach();
	return true;
}

bool FSoc::SocSend(const std::string& Msg)
{
	if(SocClient == 0)
	{
		NLOG(LOGNLP,Error,TEXT("The socket is not created"));
		return false;
	}
	NLOG(LOGNLP,Log,TEXT("[Send Msg to Py]:%s"),*TOFS(Msg));
	std::string SendMsg = Msg + "\\n";
	send(SocClient,SendMsg.c_str(),strlen(SendMsg.c_str())+1,0);
	//NLOG(LOGNLP,Log,TEXT("[Send Msg to Py]:%s"),*TOFS(SendMsg));
	return true;
}

bool FSoc::SocCmd(const std::string& Cmd, const std::string& Type, const std::string& Data)
{
	std::string CmdStr = util::Format("\"cmd\":\"{0}\",\"type\":\"{1}\",\"data\":{2}",Cmd,Type,Data);
	CmdStr = "{" + CmdStr + "}";
	return SocSend(CmdStr);
}

void FSoc::SocClose()
{
	SocCmd("soc","close","{}");
}

void FSoc::OnRecv(const std::string Msg)
{
	//NLOG(LOGNLP,Log,TEXT("[Recv Msg from Py]%s"),*TOFS(Msg));
	int Pos = Msg.find("\\n");
	if(Pos == std::string::npos)
	{
		NLOG(LOGNLP,Log,TEXT("Packets are not splitted using \\n : %s"),*TOFS(Msg));
		return;
	}
	std::string RecvData = Msg.substr(0,Pos);
	NLOG(LOGNLP,Log,TEXT("[Recv Msg from Py]%s"),*TOFS(RecvData));
	nlohmann::json Json;
	try
	{
		Json = nlohmann::json::parse(RecvData);
	}
	catch (nlohmann::detail::exception Err)
	{
		NLOG(LOGNLP,Log,TEXT("JSON string exception : %s"),*TOFS(RecvData));
		return;
	}
	if(Json.at("cmd") == "soc")
	{
		if(Json.at("type") == "close")
		{
			closesocket(SocClient);
			WSACleanup();
			IsConnected = false;
			SocClient = 0;
			NLOG(LOGNLP,Log,TEXT("[Socket closed]"));
		}
	}
}

void FSoc::Recv(const FSoc* Soc)
{
	char Buff[1024];
	int Err = 0;
	while (Soc->IsConnected)
	{
		Err = recv(Soc->SocClient,Buff,sizeof(Buff),0);
		if(Err == 0 || Err == SOCKET_ERROR)
		{
			continue;
		}
		std::string Msg = Buff;
		Soc->OnRecvCallback(Msg);
	}
}
