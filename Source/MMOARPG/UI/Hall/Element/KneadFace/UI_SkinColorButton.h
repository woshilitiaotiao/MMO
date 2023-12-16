// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_KneadFaceBase.h"
#include "UI_SkinColorButton.generated.h"

class UButton;
class UImage;
struct FMMOARPGCharacterAppearance;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_SkinColorButton : public UUI_KneadFaceBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* ColorButton;

	UPROPERTY(meta = (BindWidget))
	UImage* ColorImg;
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	virtual void InitKneadFace(const FMMOARPGCharacterAppearance* InACData);
protected:
	UFUNCTION()
	void ColorButtonClicked();
};
