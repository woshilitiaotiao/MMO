// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_SkinColorButton.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UUI_SkinColorButton::NativeConstruct()
{
	Super::NativeConstruct();

	ColorButton->OnReleased.AddDynamic(this, &UUI_SkinColorButton::ColorButtonClicked);
}

void UUI_SkinColorButton::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_SkinColorButton::ColorButtonClicked()
{

}

void UUI_SkinColorButton::InitKneadFace(const FMMOARPGCharacterAppearance* InACData)
{

}