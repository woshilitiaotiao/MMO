// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SimpleNetManage.h"
#include "MMOARPGType.h"
#include "MMOARPGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UMMOARPGGameInstance : public UGameInstance, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	virtual void Init();

	virtual void Tick(float DeltaTime);

	virtual TStatId GetStatId() const;

	virtual void Shutdown();

public:
	void CreateClient();

	void LinkServer();

	FSimpleNetManage* GetClient();
	FMMOARPGUserData &GetUserData();
	FMMOARPGGateStatus& GetGateStatus();
private:
	FSimpleNetManage* Client;
	FMMOARPGUserData UserData;
	FMMOARPGGateStatus GateStatus;
};
