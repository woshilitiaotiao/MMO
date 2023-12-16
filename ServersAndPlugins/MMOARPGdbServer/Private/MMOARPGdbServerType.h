#pragma once
#include "CoreMinimal.h"
#include "Core/SimpleMysqlLinkType.h"

struct FMysqlConfig
{
	FMysqlConfig()
		:User("root")
		,Host("127.0.0.1")
		,Pawd("root")
		,DB("hello")
		,Port(3306)
	{
		ClientFlags.Add(ESimpleClientFlags::Client_Multi_Statements);
		ClientFlags.Add(ESimpleClientFlags::Client_Multi_Results);
	}

	FString User;
	FString Host;
	FString Pawd;
	FString DB;
	int32 Port;
	TArray<ESimpleClientFlags> ClientFlags;
};