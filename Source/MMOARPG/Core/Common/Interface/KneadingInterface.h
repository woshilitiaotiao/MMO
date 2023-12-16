// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MMOARPGType.h"
#include "KneadingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UKneadingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MMOARPG_API IKneadingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	IKneadingInterface();

	virtual	void UpdateKneadingBoby() = 0;
	virtual	void UpdateKneadingBoby(const FMMOARPGCharacterAppearance& InCA) = 0;

	UFUNCTION(BlueprintCallable, Category = "Kneading")
	virtual	void SetLegSize(float InLegSize);

	UFUNCTION(BlueprintCallable, Category = "Kneading")
	virtual	void SetWaistSize(float InWaistSize);

	UFUNCTION(BlueprintCallable, Category = "Kneading")
	virtual	void SetArmSize(float InArmSize);

	UFUNCTION(BlueprintCallable, Category = "Kneading")
	virtual	float GetLegSize();

	UFUNCTION(BlueprintCallable, Category = "Kneading")
	virtual	float GetWaistSize();

	UFUNCTION(BlueprintCallable, Category = "Kneading")
	virtual	float GetArmSize();

	UFUNCTION(BlueprintCallable, Category = "Kneading")
	virtual void SetMeshPostion(USceneComponent *InMesh);

	void InitKneadingLocation(const FVector &InLocation);
protected:
	float LegSize;
	float WaistSize;
	float ArmSize;

	FVector Location;
};
