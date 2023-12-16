// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_CharacterModeling.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "../../../../Core/Hall/HallPlayerState.h"
#include "../../../../Core/Hall/HallPawn.h"
#include "../../../../Core/Hall/Character/CharacterStage.h"

void UUI_CharacterModeling::NativeConstruct()
{
	Super::NativeConstruct();

	LegSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::LegValueChanged);
	WaistSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::WaistValueChanged);
	ArmSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::ArmValueChanged);

	ModelingType->OnSelectionChanged.AddDynamic(this, &UUI_CharacterModeling::SelectModelingType);
}

void UUI_CharacterModeling::InitKneadFace(const FMMOARPGCharacterAppearance* InACData)
{
	if (InACData)
	{
		auto GetKFValue = [](float InValue)->float
		{
			return InValue / 10.f;
		};

		//����ֵ
		LegSlider->SetValue(GetKFValue(InACData->LegSize));
		WaistSlider->SetValue(GetKFValue(InACData->WaistSize));
		ArmSlider->SetValue(GetKFValue(InACData->ArmSize));

		//������ʾ��ֵ
		LegValueChanged(GetKFValue(InACData->LegSize));
		WaistValueChanged(GetKFValue(InACData->WaistSize));
		ArmValueChanged(GetKFValue(InACData->ArmSize));
	}
}

void UUI_CharacterModeling::SelectModelingType(FString SelectedItem, ESelectInfo::Type SelectionType)
{

}

void UUI_CharacterModeling::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_CharacterModeling::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_CharacterModeling::UpdatePawn()
{
	if (AHallPawn* InPawn = GetPawn<AHallPawn>())
	{
		if (AHallPlayerState* InState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AHallPlayerState>())
		{
			if (InPawn->CharacterStage)
			{
				int32 SlotID = InPawn->CharacterStage->GetSlotID();
				FMMOARPGCharacterAppearance* InCA = InState->GetCharacterCA(SlotID);
				if (!InCA)
				{
					if (auto* CurrentTmpCreateCharacter = InState->GetCurrentTmpCreateCharacter())
					{
						if (CurrentTmpCreateCharacter->SlotPosition == SlotID)
						{
							InCA = CurrentTmpCreateCharacter;
						}
					}
				}

				if (InCA) //�и������
				{
					InCA->LegSize = LegSlider->GetValue() * 10.f;
					InCA->WaistSize = WaistSlider->GetValue() * 10.f;
					InCA->ArmSize = ArmSlider->GetValue() * 10.f;
				}

				InPawn->CharacterStage->UpdateKneadingBoby();
			}
		}
	}
}

void UUI_CharacterModeling::UpdateText(UTextBlock* InValueText, float InValue)
{
	if (InValueText)
	{
		int32 IntValue = InValue * 10;
		InValueText->SetText(FText::FromString(FString::Printf(TEXT("%02d"), IntValue)));
	}
}

void UUI_CharacterModeling::LegValueChanged(float InDeltaTime)
{
	UpdateText(LegValueText, InDeltaTime);
	UpdatePawn();
}

void UUI_CharacterModeling::WaistValueChanged(float InDeltaTime)
{
	UpdateText(WaistValueText, InDeltaTime);
	UpdatePawn();
}

void UUI_CharacterModeling::ArmValueChanged(float InDeltaTime)
{
	UpdateText(ArmValueText, InDeltaTime);
	UpdatePawn();
}
