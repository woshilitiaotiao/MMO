#include "MMOARPGLoginServerObject.h"
#include "log/MMOARPGLoginServerLog.h"
#include "Protocol/LoginProtocol.h"
#include "ServerList.h"

void UMMOARPGLoginServerObject::Init()
{
	Super::Init();
}

void UMMOARPGLoginServerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UMMOARPGLoginServerObject::Close()
{
	Super::Close();

}

void UMMOARPGLoginServerObject::RecvProtocol(uint32 InProtocol)
{
	Super::RecvProtocol(InProtocol);

	switch (InProtocol)
	{
		case SP_LoginRequests:
		{
			FString AccountString;
			FString PasswordString;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_LoginRequests, AccountString,PasswordString);

			FSimpleAddrInfo AddrInfo;
			GetAddrInfo(AddrInfo);

			SIMPLE_CLIENT_SEND(dbClient, SP_LoginRequests, AccountString, PasswordString, AddrInfo);

			UE_LOG(LogMMOARPGLoginServer, Display, TEXT("[SP_LoginRequests] AccountString=%s,PasswordString=%s"), *AccountString, *PasswordString);

			break;
		}
		case SP_RegisterRequests:
		{
			FString RegisterInfo;

			//拿到客户端发送的账号
			SIMPLE_PROTOCOLS_RECEIVE(SP_RegisterRequests, RegisterInfo);

			FSimpleAddrInfo AddrInfo;
			GetAddrInfo(AddrInfo);

			SIMPLE_CLIENT_SEND(dbClient, SP_RegisterRequests, RegisterInfo, AddrInfo);

			UE_LOG(LogMMOARPGLoginServer, Display, TEXT("[SP_RegisterRequests] AccountString=%s,PasswordString=%s"), *RegisterInfo);

			break;
		}
	}
}

