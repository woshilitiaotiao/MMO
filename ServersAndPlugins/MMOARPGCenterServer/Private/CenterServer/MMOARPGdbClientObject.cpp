// Copyright (C) RenZhai.2020.All Rights Reserved.
#include "MMOARPGdbClientObject.h"
#include "log/MMOARPGCenterServerLog.h"
#include "Protocol/HallProtocol.h"
#include "Protocol/ServerProtocol.h"
#include "ServerList.h"
#include "MMOARPGType.h"
#include "MMOARPGCenterServerObject.h"

void UMMOARPGdbClientObject::Init()
{
	Super::Init();
}

void UMMOARPGdbClientObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UMMOARPGdbClientObject::Close()
{
	Super::Close();

}

void UMMOARPGdbClientObject::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

	switch (InProtocol)
	{
		case SP_PlayerRegistInfoResponses:
		{	
			FSimpleAddrInfo GateAddrInfo;
			FSimpleAddrInfo CenterAddrInfo;

			FString UserJson;
			FString SlotJson;
			SIMPLE_PROTOCOLS_RECEIVE(SP_PlayerRegistInfoResponses,UserJson,SlotJson, GateAddrInfo, CenterAddrInfo);

			if (UserJson != TEXT("[]") && SlotJson != TEXT("[]"))
			{
				if (UMMOARPGCenterServerObject *InCenterServer = Cast<UMMOARPGCenterServerObject>(FSimpleNetManage::GetNetManageNetworkObject(CenterServer, CenterAddrInfo)))
				{
					FMMOARPGPlayerRegistInfo RI;
					NetDataAnalysis::StringToCharacterAppearances(SlotJson, RI.CAInfo);
					NetDataAnalysis::StringToUserData(UserJson, RI.UserInfo);

					InCenterServer->AddRegistInfo(RI);

					//准备ds服务器
				//	FSimpleAddr DsAddr = FSimpleNetManage::GetSimpleAddr(TEXT("192.168.3.6"),7777);
					FSimpleAddr DsAddr = FSimpleNetManage::GetSimpleAddr(TEXT("192.168.3.6"), 7777);
					SIMPLE_SERVER_SEND(CenterServer,SP_LoginToDSServerResponses, CenterAddrInfo,GateAddrInfo, DsAddr);
				}
			}

			break;
		}
	}
}

