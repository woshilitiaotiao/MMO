#include "MMOARPGdbClientObject.h"
#include "log/MMOARPGLoginServerLog.h"
#include "Protocol/LoginProtocol.h"
#include "ServerList.h"
#include "MMOARPGType.h"
#include "MMOARPGGateClientObject.h"

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
		case SP_LoginResponses:
		{
			FString String;
			ELoginType Type = ELoginType::LOGIN_DB_SERVER_ERROR;
			FSimpleAddrInfo AddrInfo;
			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginResponses, AddrInfo, Type, String);
			
			FMMOARPGGateStatus GateStatus;
			if (UMMOARPGGateClientObject *InGateClient = Cast<UMMOARPGGateClientObject>(GateClientA->GetController()))
			{
				//InGateClient->GetGateStatus().GateConnetionNum
				if (InGateClient->GetGateStatus().GateConnetionNum <= 2000)
				{
					GateStatus = InGateClient->GetGateStatus();
				}	
			}

			SIMPLE_SERVER_SEND(LoginServer, SP_LoginResponses, AddrInfo,Type,String,GateStatus);

			UE_LOG(LogMMOARPGLoginServer, Display, TEXT("[SP_LoginResponses]"));

			break;
		}
		case SP_RegisterResponses:
		{
			ERegistrationType Type = ERegistrationType::SERVER_BUG_WRONG;
			FSimpleAddrInfo AddrInfo;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_RegisterResponses, AddrInfo, Type);

			SIMPLE_SERVER_SEND(LoginServer, SP_RegisterResponses, AddrInfo, Type);

			UE_LOG(LogMMOARPGLoginServer, Display, TEXT("[SP_RegisterResponses]"));
		}
	}
}

