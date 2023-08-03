#include "FSocRunnable.h"

FSocRunnable::~FSocRunnable()
{
	Stopping = true;
	UE_LOG(LOGNLPFORUE,Log,TEXT("RunnableThread destructed"));
}

bool FSocRunnable::Init()
{
	Stopping = false;
	UE_LOG(LOGNLPFORUE,Log,TEXT("RunnableThread initialized"));
	return true;
}

uint32 FSocRunnable::Run()
{
	if(!SocClient)
	{
		UE_LOG(LOGNLPFORUE,Log,TEXT("SocClient is invalid"));
		return 0;
	}
	TArray<uint8> ReceiveData;
	uint8 Element = 0;
	while(!Stopping)
	{
		ReceiveData.Init(Element,1024u);
		int32 read = 0;
		bool bRecv;
		bRecv = SocClient->Recv(ReceiveData.GetData(),ReceiveData.Num(),read);
		if(bRecv)
		{
			FString DataFString = FString(UTF8_TO_TCHAR(reinterpret_cast<const char*>(ReceiveData.GetData())));
			if(OnRecv.IsBound())
			{
				OnRecv.Execute(DataFString);
			}
			FPlatformProcess::Sleep(0.01f);
		}
	}
	return 1;
}

void FSocRunnable::Stop(){}
void FSocRunnable::Exit(){}

bool FSocRunnable::CreateSocRunnable(const FString& Name)
{
	ThreadName = Name;
	RunnableThread = MakeShareable<FRunnableThread>(FRunnableThread::Create(this,*ThreadName));
	if(RunnableThread)
	{
		UE_LOG(LOGNLPFORUE,Log,TEXT("Create RunnableThread-%s successfully!"),*ThreadName);
		return true;
	}
	else
	{
		UE_LOG(LOGNLPFORUE,Log,TEXT("Failed to create RunnableThread-%s!"),*ThreadName);
		return false;
	}
}
