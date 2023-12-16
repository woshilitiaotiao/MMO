// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMOARPGGameMode.h"
#include "MMOARPGCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMMOARPGGameMode::AMMOARPGGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
