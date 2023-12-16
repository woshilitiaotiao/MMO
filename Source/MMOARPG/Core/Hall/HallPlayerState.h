// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MMOARPGType.h"
#include "HallPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AHallPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	FCharacterAppearances& GetCharacterAppearance();

	void RemoveCharacterAppearanceBySlot(int32 InSlot);

	bool IsCharacterExistInSlot(const int32 InPos);

	FMMOARPGCharacterAppearance * GetRecentCharacter();

	FMMOARPGCharacterAppearance* GetCharacterCA(const int32 InPos);
	FMMOARPGCharacterAppearance* AddCharacterCA(const int32 InPos);
	int32 AddCharacterCA(const FMMOARPGCharacterAppearance& InPos);

	FMMOARPGCharacterAppearance* GetCurrentTmpCreateCharacter() { return &CurrentTmpCreateCharacter; }

private:
	FCharacterAppearances CharacterAppearances;
	FMMOARPGCharacterAppearance CurrentTmpCreateCharacter;
};
