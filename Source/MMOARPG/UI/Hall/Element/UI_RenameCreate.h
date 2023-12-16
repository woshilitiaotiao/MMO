// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_RenameCreate.generated.h"

class UButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_RenameCreate : public UUI_Base
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UButton* FindNameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* EditableName;

public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	void SetSlotPosition(const int32 InSlotPosition);

	void SetEditableName(const FText &InName);

	int32 GetSlotPosition() { return SlotPosition; }
protected:

	UFUNCTION()
	void ClickedCreate();

	UFUNCTION()
	void ClickedCancel();

	UFUNCTION()
	void ClickedFindName();
protected:
	int32 SlotPosition;
};
