#include "Soc.h"

DEFINE_LOG_CATEGORY(LOGNLPFORUE);

ASoc::ASoc()
{
	PrimaryActorTick.bCanEverTick = true;
}

ASoc::~ASoc()
{
}

void ASoc::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoc::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASoc::BeginDestroy()
{
	Super::BeginDestroy();
	SocClose();
}

void ASoc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ASoc::SendMsg(const FString& Msg)
{
	TSharedRef<FInternetAddr> TargetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	FString Serialized = Msg;
	bool bSend;
	TCHAR* SeriallizedChar = Serialized.GetCharArray().GetData();
	int32 Size = CalcUtf0NumFromString(SeriallizedChar);
	int32 Sent = 0;
	bSend = SocClient->SendTo((uint8*)TCHAR_TO_UTF8(SeriallizedChar),Size,Sent,*TargetAddr);
	if(bSend)
	{
		UE_LOG(LOGNLPFORUE,Log,TEXT("[To LTP | %d]: %s"),Size,*Msg);
	}
	else
	{
		UE_LOG(LOGNLPFORUE,Log,TEXT("Failed to send Msg to tlp"));
	}
	return bSend;
}

bool ASoc::SendCmd(const FString& Cmd, const FString& Type, const FString& Data)
{
	return SendMsg(CmdFormat(Cmd,Type,Data));
}

void ASoc::OnReceived(FString Data)
{
	UE_LOG(LOGNLPFORUE,Log,TEXT("OnReceived：%s"),*Data);
	OnRecvMsg.Broadcast(Data);
}

void ASoc::ConnetLTP(const FString& InIP, const int32 InPort, const FString& SocDescription)
{
	TSharedRef<FInternetAddr> ServerAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bIsValid;
	ServerAddr->SetIp(*InIP,bIsValid);
	ServerAddr->SetPort(InPort);
	if(!bIsValid)
	{
		UE_LOG(LOGNLPFORUE,Log,TEXT("Server address is invalid <%s:%d>"),*InIP,InPort);
		return;
	}

	SocClient = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream,SocDescription,false);
	int32 BuffMaxSize = 1024;
	SocClient->SetSendBufferSize(BuffMaxSize,BuffMaxSize);
	SocClient->SetReceiveBufferSize(BuffMaxSize,BuffMaxSize);
	bool bIsConnected = SocClient->Connect(*ServerAddr);
	if(bIsConnected)
	{
		UE_LOG(LOGNLPFORUE,Log,TEXT("Connected LTP Server"));
		OnConnected.Broadcast();
		SocRunnable = MakeShareable<FSocRunnable>(new FSocRunnable(SocClient));
		SocRunnable->CreateSocRunnable(TEXT("LTPReceiveThread"));
		SocRunnable->OnRecv.BindUObject(this,&ASoc::OnReceived);
	}
}

void ASoc::SocClose()
{
	if(SocRunnable)
	{
		SocRunnable->Exit();
	}
	if(SocClient)
	{
		SendCmd(TEXT("soc"),TEXT("close"),TEXT("{}"));
		SocClient->Close();
		UE_LOG(LOGNLPFORUE,Log,TEXT("SocClient closed"));
	}
}

FString ASoc::CmdFormat(const FString& Cmd, const FString& Type, const FString& Data)
{
	TArray<FStringFormatArg> Args;
	Args.Add(FStringFormatArg(Cmd));
	Args.Add(FStringFormatArg(Type));
	Args.Add(FStringFormatArg(Data));
	FString JsonStr = FString::Format(TEXT("\"cmd\":\"{0}\",\"type\":\"{1}\",\"data\":{2}"),Args);
	return TEXT("{"+JsonStr+"}");
}

int32 ASoc::CalcUtf0NumFromString(const FString& Str)
{
	int32 result = 0; 
	for (int i = 0; i < Str.Len(); i++)
	{
		if (Str[i] <= 0x7f)
			result = result + 1;
		else if (Str[i] > 0x7f && Str[i] <= 0x07ff)
			result = result + 2;
		else if (Str[i] > 0x07ff && Str[i] <= 0xffff)
			result = result + 3;
		else
			result = result + 4;
	}
	return result + 1;
}

