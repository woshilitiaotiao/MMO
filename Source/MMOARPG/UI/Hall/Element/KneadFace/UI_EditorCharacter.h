// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Core/UI_Base.h"
#include "UI_EditorCharacter.generated.h"

class UEditableTextBox;
class UButton;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_EditorCharacter : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* CharacterName;

	UPROPERTY(meta = (BindWidget))
	UButton* EditCharacterButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DeleteCharacterButton;

public:
	UUI_EditorCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct();

	virtual void NativeDestruct();

	void SetCharacterName(const FText& InName);
	void SetSlotID(int32 NewSlotID) { SlotID = NewSlotID; }
private:
	UFUNCTION()
	void EditCharacter();

	UFUNCTION()
	void DeleteCharacter();

private:
	int32 SlotID;
};
