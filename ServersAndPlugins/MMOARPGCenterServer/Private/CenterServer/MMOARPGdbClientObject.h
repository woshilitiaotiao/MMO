// Copyright (C) RenZhai.2020.All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/SimpleController.h"
#include "MMOARPGdbClientObject.generated.h"

UCLASS()
class UMMOARPGdbClientObject : public USimpleController
{
	GENERATED_BODY()

public:
	virtual void Init();
	virtual void Tick(float DeltaTime);
	virtual void Close();
	virtual void RecvProtocol(uint32 InProtocol);

};