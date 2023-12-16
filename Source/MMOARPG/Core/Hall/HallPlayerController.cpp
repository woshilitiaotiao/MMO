// Fill out your copyright notice in the Description page of Project Settings.
#include "HallPlayerController.h"

AHallPlayerController::AHallPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
}

// Called when the game starts or when spawned
void AHallPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SimpleBrowse.Register(this, NULL);
	SimpleZoom.Register(GetPawn(),400.f);
	SimplePanelMove.Register(this, GetPawn(),100.f);
}

// Called every frame
void AHallPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AHallPlayerController::ExecutionRotateCharacter()
{
	SimpleBrowse.OpenBrowsing();
}

void AHallPlayerController::StopRotateCharacter()
{
	SimpleBrowse.EndBrowsing();
}

void AHallPlayerController::ResetTarget(AActor* InTarget)
{
	SimpleBrowse.ResetTarget(InTarget);
}

void AHallPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MouseClick", IE_Released, this, &AHallPlayerController::StopRotateCharacter);

	InputComponent->BindAction("MouseRightClick", IE_Pressed, this, &AHallPlayerController::BeginMove);
	InputComponent->BindAction("MouseRightClick", IE_Released, this, &AHallPlayerController::EndMove);

	InputComponent->BindAxis("Zoom", this, &AHallPlayerController::Zoom);
}

void AHallPlayerController::Zoom(float InDeltaTime)
{
	if (InDeltaTime > 0)
	{
		SimpleZoom.ZoomPositive(InDeltaTime * 10.f);
	}
	else if (InDeltaTime < 0)
	{
		SimpleZoom.ZoomNegative(InDeltaTime * 10.f);
	}
}

void AHallPlayerController::BeginMove()
{
	SimplePanelMove.BeginMove();
}

void AHallPlayerController::EndMove()
{
	SimplePanelMove.EndMove();
}