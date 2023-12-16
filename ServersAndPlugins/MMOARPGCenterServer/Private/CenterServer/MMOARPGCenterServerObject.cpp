// Copyright (C) RenZhai.2020.All Rights Reserved.
#include "MMOARPGCenterServerObject.h"
#include "log/MMOARPGCenterServerLog.h"
#include "Protocol/ServerProtocol.h"
#include "Protocol/HallProtocol.h"
#include "ServerList.h"

void UMMOARPGCenterServerObject::Init()
{
	Super::Init();

	for (int32 i = 0; i < 2000; i++)
	{
		PlayerRegistInfos.Add(i,FMMOARPGPlayerRegistInfo());
	}
}

void UMMOARPGCenterServerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UMMOARPGCenterServerObject::Close()
{
	Super::Close();

}

void UMMOARPGCenterServerObject::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

	switch (InProtocol)
	{
		case SP_LoginToDSServerRequests:
		{
			int32 UserID = INDEX_NONE;
			int32 SlotID = INDEX_NONE;
			FSimpleAddrInfo GateAddrInfo;

			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerRequests, UserID, SlotID, GateAddrInfo)

			if (UserID != INDEX_NONE && SlotID != INDEX_NONE)
			{
				FSimpleAddrInfo CenterAddrInfo;
				GetAddrInfo(CenterAddrInfo);

				//发送ID和SlotID 请求数据
				SIMPLE_CLIENT_SEND(dbClient, SP_PlayerRegistInfoRequests, UserID, SlotID, GateAddrInfo, CenterAddrInfo);

			}

			break;
		}
		case SP_PlayerQuitRequests:
		{
			int32 UserID = INDEX_NONE;
			SIMPLE_PROTOCOLS_RECEIVE(SP_PlayerQuitRequests, UserID);

			if (UserID != INDEX_NONE)
			{
				if (RemoveRegistInfo(UserID))
				{
					UE_LOG(LogMMOARPGCenterServer, Display, TEXT("Object removed [%i] successfully"), UserID);
				}
				else
				{
					UE_LOG(LogMMOARPGCenterServer, Display, TEXT("The object was not found [%i] and may have been removed"), UserID);
				}
			}
		}
	}
}

void UMMOARPGCenterServerObject::AddRegistInfo(const FMMOARPGPlayerRegistInfo& InRegistInfo)
{
	for (auto &Tmp : PlayerRegistInfos)
	{
		if (!Tmp.Value.IsVaild())
		{
			Tmp.Value = InRegistInfo;
			break;
		}
	}
}

bool UMMOARPGCenterServerObject::RemoveRegistInfo(const int32 InUserID)
{
	for (auto& Tmp : PlayerRegistInfos)
	{
		if (Tmp.Value.UserInfo.ID == InUserID)
		{
			Tmp.Value.Reset();
			return true;
		}
	}

	return false;
}

