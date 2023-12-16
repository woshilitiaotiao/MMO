// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_Base.h"
#include "Animation/WidgetAnimation.h"

UWidgetAnimation* UUI_Base::GetNameWidgetAnimation(const FString& InWidgetAnimName)
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;

		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate(
			[&](const UWidgetAnimation* OurAnimation)
			{
				return OurAnimation->GetFName().ToString() == (InWidgetAnimName + FString("_INST"));
			});

		return *MyTempAnimation;
	}

	return NULL;
}

void UUI_Base::PlayWidgetAnim(const FString& InWidgetAnimName)
{
	if (UWidgetAnimation *MyTempAnimation = GetNameWidgetAnimation(InWidgetAnimName))
	{
		PlayAnimation(MyTempAnimation);
	}
}
