#pragma once

#include "CoreMinimal.h"
#include "UObject/SimpleController.h"
#include "MMOARPGGateServerObject.generated.h"

UCLASS()
class UMMOARPGGateServerObject : public USimpleController
{
	GENERATED_BODY()

public:
	virtual void Init();
	virtual void Tick(float DeltaTime);
	virtual void Close();
	virtual void RecvProtocol(uint32 InProtocol);

};