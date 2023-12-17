// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"

namespace SimpleHTTP
{
	namespace HTTP
	{
		class IHTTPClientRequest :public TSharedFromThis<IHTTPClientRequest>
		{
			friend struct FHTTPClient;

		public:
			IHTTPClientRequest();

			IHTTPClientRequest &operator<<(const FHttpRequestCompleteDelegate& SimpleDelegateInstance)
			{
				HttpReuest->OnProcessRequestComplete() = SimpleDelegateInstance;
				return *this;
			}

			IHTTPClientRequest &operator<<(const FHttpRequestProgressDelegate& SimpleDelegateInstance)
			{			
				HttpReuest->OnRequestProgress() = SimpleDelegateInstance;
				return *this;
			}

			IHTTPClientRequest &operator<<(const FHttpRequestHeaderReceivedDelegate& SimpleDelegateInstance)
			{
				HttpReuest->OnHeaderReceived() = SimpleDelegateInstance;
				return *this;
			}

		protected:
			bool ProcessRequest();
			void CancelRequest();

		protected:
#ifdef PLATFORM_PROJECT
#if (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 2 && ENGINE_PATCH_VERSION < 6)
			TSharedPtr<class IHttpRequest> HttpReuest;
#else
			TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> HttpReuest;
#endif
#else
			TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe> HttpReuest;//这个是4.26以上的版本

			//TSharedPtr<class IHttpRequest> HttpReuest;包含4.25以下的版本请开启这个
#endif
		};
	}
}