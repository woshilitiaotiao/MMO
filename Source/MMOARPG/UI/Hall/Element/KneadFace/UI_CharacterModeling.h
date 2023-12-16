// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_KneadFaceBase.h"
#include "UI_CharacterModeling.generated.h"

class UComboBoxString;
class USlider;
class UTextBlock;
struct FMMOARPGCharacterAppearance;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_CharacterModeling : public UUI_KneadFaceBase
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UComboBoxString* ModelingType;

	UPROPERTY(meta = (BindWidget))
		USlider* LegSlider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* LegValueText;

	UPROPERTY(meta = (BindWidget))
		USlider* WaistSlider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* WaistValueText;

	UPROPERTY(meta = (BindWidget))
		USlider* ArmSlider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ArmValueText;

public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	virtual void InitKneadFace(const FMMOARPGCharacterAppearance* InACData);
private:
	void UpdatePawn();
	void UpdateText(UTextBlock* InValueText,float InValue);
private:
	UFUNCTION()
	void LegValueChanged(float InDeltaTime);

	UFUNCTION()
	void WaistValueChanged(float InDeltaTime);

	UFUNCTION()
	void ArmValueChanged(float InDeltaTime);

	UFUNCTION()
	void SelectModelingType(FString SelectedItem, ESelectInfo::Type SelectionType);
};
