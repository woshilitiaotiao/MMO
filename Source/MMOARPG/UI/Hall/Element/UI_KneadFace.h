// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_KneadFace.generated.h"

class UCheckBox;
class UWidgetSwitcher;
struct FMMOARPGCharacterAppearance;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_KneadFace : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UCheckBox* Body;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* Appearance;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* Talent;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* FacePanel;
public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

	void InitKneadFace(const FMMOARPGCharacterAppearance* InACData);
private:

	UFUNCTION()
	void OnClickedBody(bool bClicked);

	UFUNCTION()
	void OnClickedAppearance(bool bClicked);

	UFUNCTION()
	void OnClickedTalent(bool bClicked);

};
