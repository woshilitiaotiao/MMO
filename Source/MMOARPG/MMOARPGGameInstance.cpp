// Fill out your copyright notice in the Description page of Project Settings.
#include "MMOARPGGameInstance.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "ThreadManage.h"

void UMMOARPGGameInstance::Init()
{	
	Super::Init();
}

void UMMOARPGGameInstance::Tick(float DeltaTime)
{
	if (Client)
	{
		Client->Tick(DeltaTime);
		GThread::Get()->Tick(DeltaTime);
	}
}

TStatId UMMOARPGGameInstance::GetStatId() const
{
	return TStatId();
}

void UMMOARPGGameInstance::Shutdown()
{
	Super::Shutdown();

	if (Client)
	{
		FSimpleNetManage::Destroy(Client);
		GThread::Destroy();
	}
}

void UMMOARPGGameInstance::CreateClient()
{
	FSimpleNetGlobalInfo::Get()->Init();

	Client = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);
}

void UMMOARPGGameInstance::LinkServer()
{
	if (Client)
	{
		if (!Client->Init())
		{
			delete Client;
			Client = NULL;
		}
	}
}

FSimpleNetManage* UMMOARPGGameInstance::GetClient()
{
	return Client;
}

FMMOARPGUserData& UMMOARPGGameInstance::GetUserData()
{
	return UserData;
}

FMMOARPGGateStatus& UMMOARPGGameInstance::GetGateStatus()
{
	return GateStatus;
}
