// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_KneadFaceBase.h"
#include "UI_Talent.generated.h"

struct FMMOARPGCharacterAppearance;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_Talent : public UUI_KneadFaceBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	virtual void InitKneadFace(const FMMOARPGCharacterAppearance* InACData);
};
