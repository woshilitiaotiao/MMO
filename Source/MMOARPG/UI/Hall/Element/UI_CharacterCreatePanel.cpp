// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_CharacterCreatePanel.h"
#include "Components/TextBlock.h"
#include "UI_CharacterButton.h"
#include "UI_KneadFace.h"
#include "Components/ScrollBoxSlot.h"
#include "../../../Core/Hall/HallPlayerState.h"
#include "../../../Core/Hall/Character/CharacterStage.h"
#include "../../../Core/Hall/HallPawn.h"
#include "../UI_HallMain.h"

void UUI_CharacterCreatePanel::NativeConstruct()
{
	Super::NativeConstruct();

	SlotPosition = 0;
}

void UUI_CharacterCreatePanel::NativeDestruct()
{
	Super::NativeDestruct();
}

UUI_KneadFace *UUI_CharacterCreatePanel::CreateKneadFace()
{
	List->ClearChildren();

	if (UI_KneadFaceClass)
	{
		if (UUI_KneadFace* InUI_KneadFace = CreateWidget<UUI_KneadFace>(GetWorld(), UI_KneadFaceClass))
		{
			if (UScrollBoxSlot* InScrollBoxSlot = Cast<UScrollBoxSlot>(List->AddChild(InUI_KneadFace)))
			{
				InScrollBoxSlot->SetPadding(10.f);
				return InUI_KneadFace;
			}
		}
	}

	return NULL;
}

void UUI_CharacterCreatePanel::CreateCharacterButtons()
{
	if (AHallPlayerState *InPlayerState = GetPlayerState<AHallPlayerState>())
	{
		InitCharacterButtons(InPlayerState->GetCharacterAppearance());
	}
}

void UUI_CharacterCreatePanel::InitCharacterButtons(const FCharacterAppearances& InCAs)
{
	List->ClearChildren();

	if (UI_CharacterButtonClass)
	{
		for (int32 i = 0; i < 4; i++)
		{
			if (UUI_CharacterButton* InCharacterButton = CreateWidget<UUI_CharacterButton>(GetWorld(), UI_CharacterButtonClass))
			{
				if (UScrollBoxSlot* InScrollBoxSlot = Cast<UScrollBoxSlot>(List->AddChild(InCharacterButton)))
				{
					InScrollBoxSlot->SetPadding(10.f);
				}

				InCharacterButton->SetSlotPosition(i);
				InCharacterButton->SetParents(this);

				//���ݳ�ʼ��
				if (const FMMOARPGCharacterAppearance *InCharacterAppearance = 
					InCAs.FindByPredicate([&](const FMMOARPGCharacterAppearance& InCharacterAppearance)
					{
						return InCharacterAppearance.SlotPosition == i;
					}))
				{
					InCharacterButton->InitCharacterButton(*InCharacterAppearance);
				}
			}
		}
	}
}

ACharacterStage *UUI_CharacterCreatePanel::CreateCharacter()
{
	if (CharacterStageClass)
	{
		if (AHallPawn* InPawn = GetPawn<AHallPawn>())
		{
			if (InPawn->CharacterStage)
			{
				InPawn->CharacterStage->Destroy();
			}

			InPawn->CharacterStage = GetWorld()->SpawnActor<ACharacterStage>(CharacterStageClass, SpawnPoint, FRotator::ZeroRotator);
		
			return InPawn->CharacterStage;
		}
	}

	return nullptr;
}

void UUI_CharacterCreatePanel::SpawnCharacter(const int32 InSlotPos)
{
	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
	{
		SpawnCharacter(InState->GetCharacterAppearance().FindByPredicate([&](const FMMOARPGCharacterAppearance& InCA)
		{
			return InCA.SlotPosition == InSlotPos;
		}));
	}
}

void UUI_CharacterCreatePanel::SpawnCharacter()
{
	SpawnCharacter(SlotPosition);
}

void UUI_CharacterCreatePanel::SpawnCharacter(const FMMOARPGCharacterAppearance* InACData)
{
	if (InACData)
	{
		if (CharacterStageClass)
		{
			if (ACharacterStage* InCharacter = CreateCharacter())
			{
				InCharacter->SetSlotID(InACData->SlotPosition);
				InCharacter->UpdateKneadingBoby(*InACData);
			}
		}
	}
}

void UUI_CharacterCreatePanel::SetCurrentSlotPosition(const int32 InNewPos)
{
	SlotPosition = InNewPos;

//	HighlightSelection(SlotPosition);
}

void UUI_CharacterCreatePanel::HighlightSelection(int32 InNewPos)
{
	FindByPredicateInList<UUI_CharacterButton>(
	[InNewPos](UUI_CharacterButton* InButton)->bool
	{
		if (InNewPos == InButton->GetSlotPosition())
		{
			InButton->SetHighlight(true);
		}
		else
		{
			InButton->SetHighlight(false);
		}

		return false;
	});
}

UUI_CharacterButton* UUI_CharacterCreatePanel::GetHighlightButton()
{
	return FindByPredicateInList<UUI_CharacterButton>(
		[](UUI_CharacterButton* InButton)->bool
		{
			if (InButton->IsHighlight())
			{
				return true;
			}
			else
			{
				return false;
			}
		});
}

void UUI_CharacterCreatePanel::JumpDSServer(int32 InSlotID)
{
	if (UUI_HallMain* InHallMain = GetParents<UUI_HallMain>())
	{
		InHallMain->JumpDSServer(InSlotID);
	}
}
