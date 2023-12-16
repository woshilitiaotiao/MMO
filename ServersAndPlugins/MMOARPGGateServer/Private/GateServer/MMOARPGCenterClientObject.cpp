// Copyright (C) RenZhai.2021.All Rights Reserved.
#include "MMOARPGCenterClientObject.h"
#include "log/MMOARPGGateServerLog.h"
#include "Protocol/HallProtocol.h"
#include "Protocol/GameProtocol.h"
#include "ServerList.h"
#include "MMOARPGType.h"

void UMMOARPGCenterClientObject::Init()
{
	Super::Init();
}

void UMMOARPGCenterClientObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UMMOARPGCenterClientObject::Close()
{
	Super::Close();

}

void UMMOARPGCenterClientObject::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

	switch (InProtocol)
	{
		case SP_LoginToDSServerResponses:
		{
			FString String;
			FSimpleAddrInfo GateAddrInfo;
			FSimpleAddr DSAddr;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerResponses, GateAddrInfo, DSAddr);

			SIMPLE_SERVER_SEND(GateServer, SP_LoginToDSServerResponses, GateAddrInfo, DSAddr);

			UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_LoginToDSServerResponses]"));
			break;
		}
	}
}

