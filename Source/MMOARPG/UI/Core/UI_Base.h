// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Base.generated.h"

class FSimpleChannel;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_Base : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	template<class T>
	T* GetPlayerController()
	{
		return GetWorld() != nullptr ? GetWorld()->GetFirstPlayerController<T>() : nullptr;
	}

	template<class T>
	T* GetPawn()
	{
		return GetWorld() != nullptr ? 
		(GetWorld()->GetFirstPlayerController() != nullptr ? GetWorld()->GetFirstPlayerController()->GetPawn<T>() : nullptr) : nullptr;
	}

	template<class T>
	T* GetGameInstance()
	{
		return GetWorld() != nullptr ? GetWorld()->GetGameInstance<T>() : nullptr;
	}

	template<class T>
	T* GetPlayerState()
	{
		return GetWorld() != nullptr ?
			(GetWorld()->GetFirstPlayerController() != nullptr ? GetWorld()->GetFirstPlayerController()->GetPlayerState<T>() : nullptr) : nullptr;
	}
public:
	template<class T>
	T *GetParents()
	{
		return Cast<T>(ParentWidget);
	}

	void SetParents(UWidget* InWidget) {ParentWidget = InWidget;}
	
protected:
	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel) {}

	UWidgetAnimation* GetNameWidgetAnimation(const FString& InWidgetAnimName);

public:
	void PlayWidgetAnim(const FString& InWidgetAnimName);

protected:
	UPROPERTY()
	UWidget* ParentWidget;
};
