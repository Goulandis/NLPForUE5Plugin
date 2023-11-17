#include "HttpAsyncAction.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "NLPFORUE/Common/FDefine.h"


UHttpAsyncAction* UHttpAsyncAction::HttpRequest(FString InURL, FString InData)
{
	UHttpAsyncAction* HttpAsyncAction = NewObject<UHttpAsyncAction>();
	HttpAsyncAction->URL = InURL;
	HttpAsyncAction->Data = InData;
	return HttpAsyncAction;
}

void UHttpAsyncAction::Activate()
{
	Super::Activate();
	const FHttpResponsDeleget TmpOnReqSuccess = OnReqSuccess;
	const FHttpResponsDeleget TmpOnReqFail = OnReqFail;
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest,ESPMode::ThreadSafe> HttpReq = HttpModule.CreateRequest();
	HttpReq->SetVerb(TEXT("Get"));
	HttpReq->SetHeader(TEXT("Content-Type"),TEXT("application/x-www-from-urlencoded"));
	//FString ReqData = TEXT("data=")+Data;
	//HttpReq->SetContentAsString(ReqData);
	HttpReq->SetURL(URL);
	HttpReq->OnProcessRequestComplete().BindLambda(
		[TmpOnReqSuccess,TmpOnReqFail](
			FHttpRequestPtr pRequest,
			FHttpResponsePtr pResponse,
			bool ConnectedSuccessfully
			) mutable
		{
			if(ConnectedSuccessfully)
			{
				FString ReqData = pResponse->GetContentAsString();
				TmpOnReqSuccess.Broadcast(ReqData);
			}
			else
			{
				switch (pRequest->GetStatus())
				{
					case EHttpRequestStatus::Failed_ConnectionError:
						UE_LOG(LOGNLPFORUE,Error,TEXT("Http failed to connect"));
						TmpOnReqFail.Broadcast("Http failed to connect");
						break;
					default:
						UE_LOG(LOGNLPFORUE,Error,TEXT("Http failed to request"));
						TmpOnReqFail.Broadcast("Http failed to request");
				}
			}
		}
	);
	HttpReq->ProcessRequest();
}
