// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_EditorCharacter.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "../../UI_HallMain.h"

UUI_EditorCharacter::UUI_EditorCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	,SlotID(INDEX_NONE)
{

}

void UUI_EditorCharacter::NativeConstruct()
{
	Super::NativeConstruct();

	EditCharacterButton->OnReleased.AddDynamic(this, &UUI_EditorCharacter::EditCharacter);
	DeleteCharacterButton->OnReleased.AddDynamic(this, &UUI_EditorCharacter::DeleteCharacter);
}

void UUI_EditorCharacter::NativeDestruct()
{
	Super::NativeDestruct();

	
}

void UUI_EditorCharacter::SetCharacterName(const FText& InName)
{
	CharacterName->SetText(InName);
}

void UUI_EditorCharacter::EditCharacter()
{
	if (UUI_HallMain* InHallMain = GetParents<UUI_HallMain>())
	{
		InHallMain->EditCharacter(SlotID);
	}
}

void UUI_EditorCharacter::DeleteCharacter()
{
	if (UUI_HallMain *InHallMain = GetParents<UUI_HallMain>())
	{
		InHallMain->DeleteCharacter(SlotID);
	}
}