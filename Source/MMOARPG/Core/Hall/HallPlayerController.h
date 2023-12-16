// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/SimpleBrowse.h"
#include "Core/SimpleZoom.h"
#include "Core/SimplePanelMove.h"
#include "HallPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AHallPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHallPlayerController();

	void ExecutionRotateCharacter();
	void StopRotateCharacter();
	void ResetTarget(AActor *InTarget);

	void Zoom(float InDeltaTime);

	void BeginMove();
	void EndMove();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	SimpleActorAction::FSimpleBrowse SimpleBrowse;
	SimpleActorAction::FSimpleZoom SimpleZoom;
	SimpleActorAction::FSimplePanelMove SimplePanelMove;
};
