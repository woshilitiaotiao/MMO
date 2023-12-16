// Fill out your copyright notice in the Description page of Project Settings.
#include "UI_Print.h"
#include "Components/TextBlock.h"

void UUI_Print::PlayTextAnim()
{
	PlayWidgetAnim(TEXT("PlayText"));
}

void UUI_Print::SetText(const FText& InText)
{
	MsgLog->SetText(InText);
}
