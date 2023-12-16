// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_RenameCreate.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "../UI_HallMain.h"
#include "../../../Core/Hall/HallPawn.h"
#include "../../../Core/Hall/Character/CharacterStage.h"
#include "../../../Core/Hall/HallPlayerState.h"

void UUI_RenameCreate::NativeConstruct()
{
	Super::NativeConstruct();

	SlotPosition = INDEX_NONE;

	FindNameButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedFindName);
	CreateButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedCreate);
	CancelButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedCancel);
}

void UUI_RenameCreate::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_RenameCreate::SetSlotPosition(const int32 InSlotPosition)
{
	SlotPosition = InSlotPosition;
}

void UUI_RenameCreate::SetEditableName(const FText& InName)
{
	EditableName->SetText(InName);
}

void UUI_RenameCreate::ClickedCreate()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>())
	{
		if (AHallPlayerState *InPlayerState = GetPlayerState<AHallPlayerState>())
		{
			if (auto* CurrentTmpCreateCharacter = InPlayerState->GetCurrentTmpCreateCharacter())
			{
				CurrentTmpCreateCharacter->Name = EditableName->GetText().ToString();
				CurrentTmpCreateCharacter->Date = FDateTime::Now().ToString();

				if (CurrentTmpCreateCharacter->Name.IsEmpty())
				{
					InHall->PrintLog(TEXT("Name cannot be empty."));
				}
				else
				{	
					InHall->CreateCharacter(*CurrentTmpCreateCharacter);
				}
			}			
		}
	}
}

void UUI_RenameCreate::ClickedCancel()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>())
	{
		if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
		{
			if (FMMOARPGCharacterAppearance* InCA = InState->GetCharacterCA(SlotPosition))
			{
				InHall->ResetCharacterAppearance(InCA);
			}
		}

		//ɾ���ոս�ɫ
		InHall->DestroyCharacter();

		InHall->PlayRenameOut();
		InHall->ResetCharacterCreatePanel();
	}
}

void UUI_RenameCreate::ClickedFindName()
{
	if (UUI_HallMain* InHall = GetParents<UUI_HallMain>())
	{
		FString CharacterName = EditableName->GetText().ToString();
		InHall->CheckRename(CharacterName);
	}
}
