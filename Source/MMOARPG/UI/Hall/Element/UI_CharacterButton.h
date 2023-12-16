// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "MMOARPGType.h"
#include "UI_CharacterButton.generated.h"

class UButton;
class UImage;
class UVerticalBox;
class UTextBlock;
class ACharacterStage;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_CharacterButton : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* CharacterButton;

	UPROPERTY(meta = (BindWidget))
	UImage* CreateShowIcon;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* CharacterInfo;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Date;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FLinearColor HighlightColor;
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	void SetSlotPosition(const int32 InNewPos);

	void InitCharacterButton(const FMMOARPGCharacterAppearance &InCA);

	void SetHighlight(bool bHighlight);

	bool IsHighlight();

	FORCEINLINE int32 GetSlotPosition() { return SlotPosition; }
protected:

	UFUNCTION()
	void ClickedCharacter();

public:
	
	void JumpDSServer();

protected:
	int32 SlotPosition;
	FLinearColor DefaultColor;
};
