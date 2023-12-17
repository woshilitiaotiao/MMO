#include "HTTP/SimpleHttpActionSingleRequest.h"
#include "Client/HTTPClient.h"
#include "Core/SimpleHttpMacro.h"
#include "HAL/FileManager.h"
#include "SimpleHTTPLog.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Math/UnrealMathUtility.h"

FSimpleHttpActionSingleRequest::FSimpleHttpActionSingleRequest()
	:Super()
	,Request(NULL)
{
}

bool FSimpleHttpActionSingleRequest::Suspend()
{
	return false;
}

bool FSimpleHttpActionSingleRequest::Cancel()
{
	if (Request.IsValid())
	{
		FHTTPClient().Cancel(Request.ToSharedRef());
		return true;
	}

	return false;
}

void FSimpleHttpActionSingleRequest::HttpRequestComplete(FHttpRequestPtr InRequest, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	Super::HttpRequestComplete(InRequest, Response, bConnectedSuccessfully);
}

void FSimpleHttpActionSingleRequest::HttpRequestProgress(FHttpRequestPtr InRequest, int32 BytesSent, int32 BytesReceived)
{
	Super::HttpRequestProgress(InRequest, BytesSent, BytesReceived);
}

void FSimpleHttpActionSingleRequest::HttpRequestHeaderReceived(FHttpRequestPtr InRequest, const FString& HeaderName, const FString& NewHeaderValue)
{
	Super::HttpRequestHeaderReceived(InRequest, HeaderName, NewHeaderValue);
}

void FSimpleHttpActionSingleRequest::ExecutionCompleteDelegate(FHttpRequestPtr InRequest, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	Super::ExecutionCompleteDelegate(InRequest, Response, bConnectedSuccessfully);

	//对于单个HTTP请求 就这样执行就行
	AllRequestCompleteDelegate.ExecuteIfBound();
	AllTasksCompletedDelegate.ExecuteIfBound();

	bRequestComplete = true;
}

bool FSimpleHttpActionSingleRequest::GetObject(const FString& URL)
{
	bSaveDisk = false;

	Request = MakeShareable(new FGetObjectRequest(URL));

	REQUEST_BIND_FUN(FSimpleHttpActionSingleRequest)

	return FHTTPClient().Execute(Request.ToSharedRef());
}

bool FSimpleHttpActionSingleRequest::GetObject(const FString& URL, const FString& SavePaths)
{
	TmpSavePaths = SavePaths;

	Request = MakeShareable(new FGetObjectRequest(URL));

	REQUEST_BIND_FUN(FSimpleHttpActionSingleRequest)

	return FHTTPClient().Execute(Request.ToSharedRef());
}

bool FSimpleHttpActionSingleRequest::PutObject(const FString& URL, const TArray<uint8>& Data)
{
	Request = MakeShareable(new FPutObjectRequest(URL, Data));

	REQUEST_BIND_FUN(FSimpleHttpActionSingleRequest)

	return FHTTPClient().Execute(Request.ToSharedRef());
}

bool FSimpleHttpActionSingleRequest::PutObject(const FString& URL, const FString& LocalPaths)
{
	TArray<uint8> Data;
	if (FFileHelper::LoadFileToArray(Data, *LocalPaths))
	{
		Request = MakeShareable(new FPutObjectRequest(URL, Data));

		REQUEST_BIND_FUN(FSimpleHttpActionSingleRequest)

		return FHTTPClient().Execute(Request.ToSharedRef());
	}

	return false;
}

bool FSimpleHttpActionSingleRequest::PutObjectByString(const FString& URL, const FString& InBuff)
{
	Request = MakeShareable(new FPutObjectRequest(URL, InBuff));

	REQUEST_BIND_FUN(FSimpleHttpActionSingleRequest)

	return FHTTPClient().Execute(Request.ToSharedRef());
}

bool FSimpleHttpActionSingleRequest::PutObject(const FString& URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	Request = MakeShareable(new FPutObjectRequest(URL, Stream));

	REQUEST_BIND_FUN(FSimpleHttpActionSingleRequest)

	return FHTTPClient().Execute(Request.ToSharedRef());
}

bool FSimpleHttpActionSingleRequest::DeleteObject(const FString& URL)
{
	Request = MakeShareable(new FDeleteObjectsRequest(URL));
	return FHTTPClient().Execute(Request.ToSharedRef());
}

bool FSimpleHttpActionSingleRequest::PostObject(const FString& URL)
{
	//FString NewURL = FGenericPlatformHttp::UrlEncode(URL);

	bSaveDisk = false;

	Request = MakeShareable(new FPostObjectsRequest(URL));

	REQUEST_BIND_FUN(FSimpleHttpActionSingleRequest)

	return FHTTPClient().Execute(Request.ToSharedRef());
}
