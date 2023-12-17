// Copyright (C) RenZhai.2020.All Rights Reserved.
#include "Client/HTTPClient.h"

SimpleHTTP::HTTP::FHTTPClient::FHTTPClient()
{

}

SimpleHTTP::HTTP::FHTTPClient::FHTTPClient(TSharedPtr<IHTTPClientRequest> InHTTPRequest)
{
	Execute(InHTTPRequest.ToSharedRef());
}

bool SimpleHTTP::HTTP::FHTTPClient::Execute(TSharedRef<IHTTPClientRequest> InHTTPRequest) const
{
	return InHTTPRequest->ProcessRequest();
}

void SimpleHTTP::HTTP::FHTTPClient::Cancel(TSharedRef<IHTTPClientRequest> InHTTPRequest)
{
	InHTTPRequest->CancelRequest();
}
