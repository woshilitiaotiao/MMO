// Copyright (C) RenZhai.2020.All Rights Reserved.

#include "HTTP/Core/SimpleHttpActionRequest.h"
#include "Client/HTTPClient.h"
#include "Core/SimpleHttpMacro.h"
#include "HAL/FileManager.h"
#include "SimpleHTTPLog.h"
#include "Misc/Paths.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/FileHelper.h"
//#include "GenericPlatform/GenericPlatformHttp.h"

FSimpleHttpActionRequest::FSimpleHttpActionRequest()
	:bRequestComplete(false)
	,bSaveDisk(true)
{
}

FSimpleHttpActionRequest::~FSimpleHttpActionRequest()
{

}

bool FSimpleHttpActionRequest::Suspend()
{
	checkf(0,TEXT("UE HTTP currently does not support single pause. However, we support the suspension of the entire HTTP download!"));
	
	return false;
}

bool FSimpleHttpActionRequest::Cancel()
{
	return false;
}

void FSimpleHttpActionRequest::GetObjects(const TArray<FString> &URL, const FString &SavePaths)
{

}

void FSimpleHttpActionRequest::GetObjects(const TArray<FString> &URL)
{

}

void FSimpleHttpActionRequest::DeleteObjects(const TArray<FString> &URL)
{

}

void FSimpleHttpActionRequest::HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	FString DebugPram;
	Request->GetURLParameter(DebugPram);
	UE_LOG(LogSimpleHTTP, Warning,
		TEXT("HttpRequestComplete URL=%s,Param=%s"),
		*Request->GetURL(),
		*DebugPram);

	//404 405 100 -199 200 -299
	if (!Request.IsValid())
	{
		ExecutionCompleteDelegate(Request, Response, bConnectedSuccessfully);

		UE_LOG(LogSimpleHTTP, Warning, TEXT("Server request failed."));
	}
	else if(!Response.IsValid())
	{
		ExecutionCompleteDelegate(Request, Response, bConnectedSuccessfully);
		UE_LOG(LogSimpleHTTP, Warning, TEXT(" Response failed."));
	}
	else if (!bConnectedSuccessfully)
	{
		ExecutionCompleteDelegate(Request, Response, bConnectedSuccessfully);
		UE_LOG(LogSimpleHTTP, Warning, TEXT("HTTP connected failed. msg[%s]"),
			*Response->GetContentAsString());
	}
	else if (!EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		ExecutionCompleteDelegate(Request, Response, bConnectedSuccessfully);
		UE_LOG(LogSimpleHTTP, Warning, TEXT("HTTP error code [%d] Msg[%s]."), 
			Response->GetResponseCode(),
			*Response->GetContentAsString());
	}
	else
	{
		if (Request->GetVerb() == "GET")
		{
			if (bSaveDisk)
			{
				FString Filename = FPaths::GetCleanFilename(Request->GetURL());
				FFileHelper::SaveArrayToFile(Response->GetContent(), *(GetPaths() / Filename));

				UE_LOG(LogSimpleHTTP, Log, TEXT("Store the obtained http file locally."));
				UE_LOG(LogSimpleHTTP, Log, TEXT("%s."), *Filename);
			}
			else
			{
				UE_LOG(LogSimpleHTTP, Log, TEXT("This is a get request that is not stored locally."));
			}
		}

		ExecutionCompleteDelegate(Request, Response, bConnectedSuccessfully);
		UE_LOG(LogSimpleHTTP, Log, TEXT("Request to complete execution of binding agent."));
	}
}

void FSimpleHttpActionRequest::HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	FSimpleHttpRequest SimpleHttpRequest;
	RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

	SimpleHttpRequestProgressDelegate.ExecuteIfBound(SimpleHttpRequest, BytesSent, BytesReceived);
	SimpleSingleRequestProgressDelegate.ExecuteIfBound(SimpleHttpRequest, BytesSent, BytesReceived);

//	UE_LOG(LogSimpleHTTP, Log, TEXT("Http request progress."));
}

void FSimpleHttpActionRequest::HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
{
	FSimpleHttpRequest SimpleHttpRequest;
	RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

	SimpleHttpRequestHeaderReceivedDelegate.ExecuteIfBound(SimpleHttpRequest, HeaderName, NewHeaderValue);
	SimpleSingleRequestHeaderReceivedDelegate.ExecuteIfBound(SimpleHttpRequest, HeaderName, NewHeaderValue);

//	UE_LOG(LogSimpleHTTP, Log, TEXT("Http request header received."));
}

void FSimpleHttpActionRequest::Print(const FString &Msg, float Time /*= 10.f*/, FColor Color /*= FColor::Red*/)
{
#ifdef PLATFORM_PROJECT
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Time, Color, Msg);
	}
#endif
}

void FSimpleHttpActionRequest::ExecutionCompleteDelegate(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	FSimpleHttpRequest SimpleHttpRequest;
	FSimpleHttpResponse SimpleHttpResponse;
	ResponsePtrToSimpleResponse(Response, SimpleHttpResponse);
	RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

	SimpleHttpRequestCompleteDelegate.ExecuteIfBound(SimpleHttpRequest, SimpleHttpResponse, bConnectedSuccessfully);
	SimpleCompleteDelegate.ExecuteIfBound(SimpleHttpRequest, SimpleHttpResponse, bConnectedSuccessfully);
}

bool FSimpleHttpActionRequest::GetObject(const FString &URL, const FString &SavePaths)
{
	return false;
}

bool FSimpleHttpActionRequest::GetObject(const FString &URL)
{
	return false;
}

bool FSimpleHttpActionRequest::PutObject(const FString &URL,const TArray<uint8> &Data)
{
	return false;
}

bool FSimpleHttpActionRequest::PutObject(const FString &URL, const FString &LocalPaths)
{
	return false;
}

bool FSimpleHttpActionRequest::PutObjectByString(const FString& URL, const FString& InBuff)
{
	return false;
}

bool FSimpleHttpActionRequest::PutObject(const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	return false;
}

bool FSimpleHttpActionRequest::DeleteObject(const FString &URL)
{
	return false;
}

bool FSimpleHttpActionRequest::PostObject(const FString &URL)
{
	return false;
}