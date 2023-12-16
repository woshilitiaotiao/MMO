#include "MMOARPGGateServerObject.h"
#include "log/MMOARPGGateServerLog.h"
#include "Protocol/ServerProtocol.h"
#include "Protocol/HallProtocol.h"
#include "MMOARPGType.h"
#include "ServerList.h"

void UMMOARPGGateServerObject::Init()
{
	Super::Init();
}

void UMMOARPGGateServerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UMMOARPGGateServerObject::Close()
{
	Super::Close();

}

void UMMOARPGGateServerObject::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

	switch (InProtocol)
	{
		case SP_GateStatusRequests:
		{
			FMMOARPGGateStatus Status;
			
			GetServerAddrInfo(Status.GateServerAddrInfo);

			Status.GateConnetionNum = GetManage()->GetConnetionNum();

			SIMPLE_PROTOCOLS_SEND(SP_GateStatusResponses, Status);

			UE_LOG(LogMMOARPGGateServer, Display, TEXT("SP_GateStatusRequests"));
			break;
		}
		case SP_CharacterAppearanceRequests:
		{
			int32 PlayerID = INDEX_NONE;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_CharacterAppearanceRequests, PlayerID);

			FSimpleAddrInfo AddrInfo;
			GetAddrInfo(AddrInfo);

			SIMPLE_CLIENT_SEND(dbClient, SP_CharacterAppearanceRequests, PlayerID, AddrInfo);

			UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_CharacterAppearanceRequests] UserID = %i"), PlayerID);

			break;
		}
		case SP_CheckCharacterNameRequests:
		{
			int32 PlayerID = INDEX_NONE;
			FString CharacterName;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_CheckCharacterNameRequests, PlayerID, CharacterName);

			FSimpleAddrInfo AddrInfo;
			GetAddrInfo(AddrInfo);

			SIMPLE_CLIENT_SEND(dbClient, SP_CheckCharacterNameRequests, PlayerID, CharacterName, AddrInfo);

			UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_CheckCharacterNameRequests] UserID = %i"), PlayerID);
			break;
		}
		case SP_CreateCharacterRequests:
		{
			int32 PlayerID = INDEX_NONE;
			FString JsonString;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_CreateCharacterRequests, PlayerID, JsonString);

			FSimpleAddrInfo AddrInfo;
			GetAddrInfo(AddrInfo);
			SIMPLE_CLIENT_SEND(dbClient, SP_CreateCharacterRequests, PlayerID, JsonString, AddrInfo);

			UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_CreateCharacterRequests] UserID = %i"), PlayerID);
			break;
		}
		case SP_DeleteCharacterRequests:
		{
			int32 PlayerID = INDEX_NONE;
			int32 SlotID = INDEX_NONE;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_DeleteCharacterRequests, PlayerID, SlotID);

			FSimpleAddrInfo AddrInfo;
			GetAddrInfo(AddrInfo);
			SIMPLE_CLIENT_SEND(dbClient, SP_DeleteCharacterRequests, PlayerID, SlotID, AddrInfo);

			UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_DeleteCharacterRequests] UserID = %i SlotID=%i"), PlayerID, SlotID);
			break;
		}
		case SP_EditorCharacterRequests:
		{
			int32 PlayerID = INDEX_NONE;
			FString JsonString;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_EditorCharacterRequests, PlayerID, JsonString);

			FSimpleAddrInfo AddrInfo;
			GetAddrInfo(AddrInfo);

			SIMPLE_CLIENT_SEND(dbClient, SP_EditorCharacterRequests, PlayerID, JsonString, AddrInfo);

			UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_EditorCharacterRequests] UserID = %i JsonString=[%s]"), PlayerID, *JsonString);
			break;
		}
		case SP_LoginToDSServerRequests:
		{
			int32 PlayerID = INDEX_NONE;
			int32 SlotID = INDEX_NONE;

			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerRequests, PlayerID, SlotID);

			FSimpleAddrInfo AddrInfo;
			GetAddrInfo(AddrInfo);
			SIMPLE_CLIENT_SEND(CenterClient, SP_LoginToDSServerRequests, PlayerID, SlotID, AddrInfo);

			UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_LoginToDSServerRequests] UserID = %i SlotID = %i"), PlayerID, SlotID);
		}
	}
}

