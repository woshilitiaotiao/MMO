// Copyright (C) RenZhai.2021.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/SimpleController.h"
#include "MMOARPGCenterClientObject.generated.h"

UCLASS()
class UMMOARPGCenterClientObject : public USimpleController
{
	GENERATED_BODY()

public:
	virtual void Init();
	virtual void Tick(float DeltaTime);
	virtual void Close();
	virtual void RecvProtocol(uint32 InProtocol);

};