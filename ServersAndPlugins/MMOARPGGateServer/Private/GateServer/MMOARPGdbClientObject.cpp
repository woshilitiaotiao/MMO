#include "MMOARPGdbClientObject.h"
#include "log/MMOARPGGateServerLog.h"
#include "Protocol/HallProtocol.h"
#include "ServerList.h"
#include "MMOARPGType.h"

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
	case SP_CharacterAppearanceResponses:
	{
		FString String;
		FSimpleAddrInfo AddrInfo;

		//拿到客户端发送的账号
		SIMPLE_PROTOCOLS_RECEIVE(SP_CharacterAppearanceResponses, AddrInfo, String);

		SIMPLE_SERVER_SEND(GateServer, SP_CharacterAppearanceResponses, AddrInfo, String);

		UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_CharacterAppearanceResponses]"));
		break;
	}
	case SP_CheckCharacterNameResponses:
	{
		FSimpleAddrInfo AddrInfo;
		ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;

		//拿到客户端发送的账号
		SIMPLE_PROTOCOLS_RECEIVE(SP_CheckCharacterNameResponses, CheckNameType, AddrInfo);

		SIMPLE_SERVER_SEND(GateServer, SP_CheckCharacterNameResponses, AddrInfo, CheckNameType);

		UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_CheckCharacterNameResponses]"));
		break;
	}
	case SP_CreateCharacterResponses:
	{
		FSimpleAddrInfo AddrInfo;
		ECheckNameType NameType = ECheckNameType::UNKNOWN_ERROR;
		bool bCreateCharacter = false;
		FString JsonString;

		//拿到客户端发送的账号
		SIMPLE_PROTOCOLS_RECEIVE(SP_CreateCharacterResponses, NameType, bCreateCharacter, JsonString, AddrInfo);

		SIMPLE_SERVER_SEND(GateServer, SP_CreateCharacterResponses, AddrInfo, NameType, bCreateCharacter, JsonString);

		UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_CreateCharacterResponses]"));
		break;
	}
	case SP_DeleteCharacterResponses:
	{
		int32 UserID = INDEX_NONE;
		FSimpleAddrInfo AddrInfo;
		int32 SlotID = INDEX_NONE;
		int32 SuccessDeleteCount = 0;

		//拿到客户端发送的账号
		SIMPLE_PROTOCOLS_RECEIVE(SP_DeleteCharacterResponses, UserID, SlotID, SuccessDeleteCount, AddrInfo);

		SIMPLE_SERVER_SEND(GateServer, SP_DeleteCharacterResponses, AddrInfo, UserID, SlotID, SuccessDeleteCount);

		UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_DeleteCharacterResponses]"));
		break;
	}
	case SP_EditorCharacterResponses:
	{
		FSimpleAddrInfo AddrInfo;
		bool bUpdateSucceeded = false;

		//拿到客户端发送的账号
		SIMPLE_PROTOCOLS_RECEIVE(SP_EditorCharacterResponses, bUpdateSucceeded, AddrInfo);

		SIMPLE_SERVER_SEND(GateServer, SP_EditorCharacterResponses, AddrInfo, bUpdateSucceeded);

		UE_LOG(LogMMOARPGGateServer, Display, TEXT("[SP_EditorCharacterResponses]"));
		break;
	}
	}
}

