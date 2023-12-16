// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStage.h"
#include "Components/CapsuleComponent.h"
#include "../HallPlayerController.h"
#include "../HallPlayerState.h"

// Sets default values
ACharacterStage::ACharacterStage()
	:SlotID(INDEX_NONE)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterStage::BeginPlay()
{
	Super::BeginPlay();

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->OnClicked.AddDynamic(this, &ACharacterStage::OnClicked);
	}

	InitKneadingLocation(GetMesh()->GetComponentLocation());
}

// Called every frame
void ACharacterStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterStage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterStage::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	if (AHallPlayerController* InPlayerController = GetWorld()->GetFirstPlayerController<AHallPlayerController>())
	{
		InPlayerController->ResetTarget(this);
		InPlayerController->ExecutionRotateCharacter();
	}
}

void ACharacterStage::UpdateKneadingBoby(const FMMOARPGCharacterAppearance& InCA)
{
	SetLegSize(InCA.LegSize);
	SetWaistSize(InCA.WaistSize);
	SetArmSize(InCA.ArmSize);

	//需要Mesh也要和我们的骨骼一致变化
	SetMeshPostion(GetMesh());
}

void ACharacterStage::UpdateKneadingBoby()
{
	if (SlotID != INDEX_NONE)
	{
		if (AHallPlayerState* InState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AHallPlayerState>())
		{
			if (FMMOARPGCharacterAppearance *InCA = InState->GetCharacterCA(SlotID))
			{
				UpdateKneadingBoby(*InCA);
			}
			else if(InState->GetCurrentTmpCreateCharacter() && InState->GetCurrentTmpCreateCharacter()->SlotPosition == SlotID)
			{
				UpdateKneadingBoby(*InState->GetCurrentTmpCreateCharacter());
			}
		}
	}
}

void ACharacterStage::SetSlotID(int32 InID)
{
	SlotID = InID;
}