// Fill out your copyright notice in the Description page of Project Settings.


#include "HallPlayerState.h"

FCharacterAppearances& AHallPlayerState::GetCharacterAppearance()
{
	return CharacterAppearances;
}

void AHallPlayerState::RemoveCharacterAppearanceBySlot(int32 InSlot)
{
	FMMOARPGCharacterAppearance CA;
	CA.SlotPosition = InSlot;

	CharacterAppearances.Remove(CA);
}

bool AHallPlayerState::IsCharacterExistInSlot(const int32 InPos)
{
	return GetCharacterCA(InPos) != nullptr;
}

FMMOARPGCharacterAppearance* AHallPlayerState::GetRecentCharacter()
{
	FDateTime MaxDateTime;
	int32 Index = INDEX_NONE;
	for (int32 i =0;i < CharacterAppearances.Num();i++)
	{
		FDateTime DateTime;
		FDateTime::Parse(CharacterAppearances[i].Date, DateTime);
		if (DateTime > MaxDateTime)
		{
			MaxDateTime = DateTime;
			Index = i;
		}
	}

	if (Index == INDEX_NONE)
	{
		return nullptr;
	}

	return &CharacterAppearances[Index];
}

FMMOARPGCharacterAppearance* AHallPlayerState::GetCharacterCA(const int32 InPos)
{
	return CharacterAppearances.FindByPredicate([InPos](const FMMOARPGCharacterAppearance& InCA) {return InPos == InCA.SlotPosition; });
}

FMMOARPGCharacterAppearance* AHallPlayerState::AddCharacterCA(const int32 InPos)
{
	FMMOARPGCharacterAppearance* InCAInstance = nullptr;
	if (FMMOARPGCharacterAppearance *InCA = GetCharacterCA(InPos))
	{
		InCAInstance = InCA;
	}
	else
	{
		CharacterAppearances.Add(FMMOARPGCharacterAppearance());
		FMMOARPGCharacterAppearance &InCARef = CharacterAppearances.Last();
		InCARef.SlotPosition = InPos;

		InCAInstance = &InCARef;
	}

	return InCAInstance;
}

int32 AHallPlayerState::AddCharacterCA(const FMMOARPGCharacterAppearance& InCA)
{
	return CharacterAppearances.Add(InCA);
}